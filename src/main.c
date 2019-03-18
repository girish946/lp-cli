#include<stdio.h>
#include<stdlib.h>
#include<argp.h>
#include<string.h>
#include <curl/curl.h>
#include"jsonp.h"
#include"paste.h"
#include"host.h"

const char *argp_program_version = "lp-cli 1.0.0";

const char *argp_program_bug_address = "girish946@gmail.com";

static char doc[] = "Cli client for local-paste\
 https://github.com/girish946/local-paste";

static char args_doc[] = "ARG1";

static struct argp_option options[] = {
    {"verbose",  'v', 0, 0, "Produce verbose output"},
    {"name",     'n', "Name", 0, "Paste name"},
    {"content",  'c', "CONTENT", 0, "Paste content"},
    {"id",       'i', "UUID", 0, "PasteId for fetching the paste"},
    {"server",   's', "SERVER", 0, "server ip"},
    {"port",     'p', "PORT", 0, "server port"},
    {0}
};

struct arguments{
    char *args[1];
    int verbose;
    char *name;
    char *content;
    char *id;
    char *server;
    char *port;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state){
    struct arguments *arguments = state->input; 
    switch (key){
        case 'v':
            arguments->verbose = 1;
            break;
        case 'n': 
            arguments->name = arg;
            break;
        case 'c':
            arguments->content = arg;
            break;
        case 'i':
            arguments->id = arg;
            break;
        case 's':
            arguments->server = arg;
            break;
        case 'p':
            arguments->port = arg;
            break;
        case ARGP_KEY_ARG:
            if(state->arg_num >1)
                argp_usage(state);
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if(state->arg_num <1)
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp= {options, parse_opt, args_doc, doc};

int getAllPastes(void){
    char url[256];
    sprintf(url, "%s:%s/api/selectDb", host, port);
    printf("\n %s", url);
    struct MemoryStruct ms;
    ms.memory = malloc(1);
    ms.size = 0;
    getJson(url, (void *)&ms);
    // printf("\n%s", ms.memory);

    //initialize json parser
    json_char* json;
    json_value* value;
    json = (json_char*)ms.memory;

    //parse json
    value = json_parse(json,ms.size);
    printf("\n name: %s\n",
            value->u.object.values[0].name);

    json_value* paste_1 = value->u.object.values[0].value;
    printf("\n Total Pastes found: %d\n", paste_1->u.array.length);

    struct Pastes pastes;
    pastes.length = paste_1->u.array.length;


    int length = paste_1->u.array.length;
    pastes.paste_array = (struct Paste*)malloc(sizeof(struct Paste)*length);
    int x=0;
    for (x = 0; x < length; x++) {
        process_value(paste_1->u.array.values[x], 0,
                      (void*)&pastes.paste_array[x]);
    }

    for(int i=0; i<length; i++){
        printf("\n %d: %s: %s", i, pastes.paste_array[i].name,
                                   pastes.paste_array[i].id);
    }

    return 0;
}

int getPaste(char* id){

    // generate url from pasteId
    char url[265];
    sprintf(url, "%s:%s/api/get/%s",host,port,id);

    // initialize memory structure for http request
    struct MemoryStruct ms;
    ms.memory = malloc(1);
    ms.size = 0;

    //make http request
    getJson(url,  (void *)&ms);

    // init a new paste struct
    struct Paste p;

    //initialize json parser
    json_char* json;
    json_value* value;
    json = (json_char*)ms.memory;

    //parse json
    value = json_parse(json,ms.size);

    if (value == NULL) {
            fprintf(stderr, "Unable to parse data\n");
            //free(ms);
            exit(1);
    }

    // set values in paste struct from parsed json struct.
    process_value(value, 0, (void*)&p);

    //free allocated memory
    json_value_free(value);
    free(ms.memory);

    //print recived data
    printf("\nname:      %s\n", p.name);
    printf("content:     \n%s\n", p.content);
    printf("\nid:        %s\n", p.id);
    printf("timestamp: %s\n", p.timestamp);
    return 0;
}

int main(int argc, char* args[]){

    struct arguments arguments;

    arguments.verbose = 0;
    arguments.name =    "";
    arguments.content = "";
    arguments.id =      "";
    arguments.server =  "";
    arguments.port =    "";

    argp_parse(&argp, argc, args, 0, 0, &arguments);
    /*printf(" ARG1 =%s\n VERBOSE=%s\n NAME=%s\n CONTENT=%s\n\
 ID=%s\n", arguments.args[0], arguments.verbose ? "yes" : "no", 
    arguments.name, arguments.content, arguments.id);*/
    if(strlen(arguments.server)> 0 ){
        printf("\n need to change the host");
    }

    if(strcmp("GET", arguments.args[0]) == 0){
        if(strcmp("all", arguments.id) != 0){
            getPaste(arguments.id);
            return 0;
        }else{
            printf("\n getting all pastes from");
            getAllPastes();
        }

    }else{
        printf("try again\n");
    }
    return 0;
}
