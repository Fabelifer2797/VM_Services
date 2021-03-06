#include <stdio.h>
#include <ulfius.h>
#include <string.h>

struct Config {
    char puerto [256];
    char dirColores [256];
    char dirHistograma [256];
    char dirLog [256];
};

struct Config config;

void readConf() {
    FILE * file;
    char line[256];
    char * token;
    char * seps = ":\n";
    file = fopen("/etc/server/config.conf", "r");
    if (file == NULL) {
        perror("Error opening the file");
    }
    // Read config files lines
    while (fgets(line, 256, file)) {
        char key [256], val [256];
        // Skip if first char is a comment
        if (line[0] == '#') {
            continue;
        }
        // Read token delimited by colon
        token = strtok(line, seps);
        for (int i = 0; token != NULL; i++) {
            // Set value to key
            if (i == 0) {
                strcpy(key, token);
            }
            // Set value to val
            else if (i == 1) {
                strcpy(val, token);
            }
            token = strtok(NULL, seps);
        }
        // Set values to config struct
        if (strcmp(key, "Puerto") == 0) {
            strcpy(config.puerto, val);
        }
        else if (strcmp(key, "DirColores") == 0) {
            strcpy(config.dirColores, val);
        }
        else if (strcmp(key, "DirHisto") == 0) {
            strcpy(config.dirHistograma, val);
        }
        else if (strcmp(key, "DirLog") == 0) {
            strcpy(config.dirLog, val);
        }
        else {
            printf("Case: %s not found\n", key);
        }
    }
}

/**
 * Callback function for
 */
int callback_options(const struct _u_request * request, struct _u_response * response, void * user_data) {
    u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
    u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
    u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
    return U_CALLBACK_COMPLETE;
}

int callback_post (const struct _u_request * request, struct _u_response * response, void * user_data) {
    printf("Recibiendo mensaje...\n");
    json_error_t j_err;
    size_t index;
    json_t * json_request = ulfius_get_json_body_request(request, &j_err), * json_response = NULL, * value = NULL;
    if (!json_request) {
        printf("error: %s \n", j_err.text);
        printf("error: %s \n", j_err.source);
        printf("error: %i \n", j_err.line);
    }
    // Read each json array element and download its respective image
    json_array_foreach(json_request, index, value) {
        const char * name = json_string_value(json_object_get(value, "name"));
        const char * link = json_string_value(json_object_get(value, "link"));
        const char * username = json_string_value(json_object_get(value, "username"));
        const char * sel_function = json_string_value(json_object_get(value, "selected_function"));
        char * command;
        // Applies histogram function to the image
        if (strcmp(sel_function, "1") == 0) {
            command = msprintf("curl %s --output %s/%s", link, config.dirHistograma, name);
            system(command);
            char HE_path [256] = "python3 ~/servidor/PythonScripts/HistogramEqualization.py";
            char buffer [256];
            snprintf(buffer, sizeof(buffer), "%s %s %s", HE_path,config.dirHistograma,config.dirHistograma );
            system(buffer);


        }
        // Applies color function to the image
        else if (strcmp(sel_function, "2") == 0) {
            command = msprintf("curl %s --output %s/%s", link, config.dirColores, name);
            system(command);
            char RGB_path [256] = "python3 ~/servidor/PythonScripts/RGB_Classification.py.py";
            char buffer [256];
            snprintf(buffer, sizeof(buffer), "%s %s %s", RGB_path,config.dirColores,config.dirColores );
            system(buffer);
        }
        else {
            command = msprintf("curl %s --output %s", link, name);
        }
        printf("comando: %s\n", command);
        //system(command);
    }
    // Create response json
    json_response = json_object();
    json_object_set_new(json_response, "message", json_string("Correcto"));
    ulfius_set_json_body_response(response, 200, json_response);
    // Destroy object references
    json_decref(json_response);
    json_decref(json_request);
    return U_CALLBACK_CONTINUE;
}

/**
 * main function
 */
int main(void) {
    
    struct _u_instance instance;
    char * rest;
    // Read config.conf and set values to config struct
    readConf();

    int port = (int) strtol(config.puerto, &rest, 10);
    // Initialize instance with the port number
    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return(1);
    }

    // callback functions declarations
    ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &callback_options, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/contraste", NULL, 0, &callback_post, NULL);

    // Allow all origins
    u_map_put(instance.default_headers, "Access-Control-Allow-Origin", "*");
    u_map_put(instance.default_headers, "Access-Control-Allow-Credentials", "true");
    u_map_put(instance.default_headers, "Cache-Control", "no-store");
    u_map_put(instance.default_headers, "Pragma", "no-cache");

    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);
        // Wait for the user to press <enter> on the console to quit the application
        getchar();
    } else {
        fprintf(stderr, "Error starting framework\n");
    }
    printf("End framework\n");

    // Stops the framework
    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
}
