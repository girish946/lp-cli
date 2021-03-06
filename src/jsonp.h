/* vim: set et ts=4
 *
 * Copyright (C) 2015 Mirko Pasqualetti  All rights reserved.
 * https://github.com/udp/json-parser
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include"json.h"
#include"paste.h"
//#include"jsonp.h"

/*
 * Test for json.c
 *
 * Compile (static linking) with
 *         gcc -o test_json -I.. test_json.c ../json.c -lm
 *
 * Compile (dynamic linking) with
 *         gcc -o test_json -I.. test_json.c -lm -ljsonparser
 *
 * USAGE: ./test_json <json_file>
 */

static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
                printf(" ");
        }
}

static void process_value(json_value* value, int depth, void* paste);

static void process_object(json_value* value, int depth, void* paste)
{
        struct Paste* p = (struct Paste*)paste;
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                int strl = strlen(
                             value->u.object.values[x].value->u.string.ptr);

                print_depth_shift(depth);
                if(strcmp(value->u.object.values[x].name, "Name") == 0){
                    p->name = malloc(strl);
                    memcpy(p->name,
                           value->u.object.values[x].value->u.string.ptr,
                           strl);
                }
                else{
                    if(strcmp(value->u.object.values[x].name, "Id") == 0){

                        p->id = malloc(strl);
                        memcpy(
                            p->id, 
                            value->u.object.values[x].value->u.string.ptr, 
                            strl
                            );
                    }else{
                        if(strcmp(
                            value->u.object.values[x].name,
                            "Content") == 0){

                            p->content = malloc(strl);
                            memcpy(
                                p->content,
                                value->u.object.values[x].value->u.string.ptr,
                                strl
                                );
                    }else{
                        if(strcmp(
                                value->u.object.values[x].name,
                                "TimeStamp"
                                ) == 0
                            ){

                            p->timestamp = malloc(strl);
                            memcpy(
                                  p->timestamp,
                                  value->u.object.values[x].value->u.string.ptr, 
                                  strl
                                );
                        }else{
                            if(strcmp(
                                     value->u.object.values[x].name, "pastes")
                                    == 0){
                                printf("\n it is an array");
                            }
                        }
                    }
                }
            }
        }
}

static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        printf("array\n");
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth, NULL);
        }
}

static void process_value(json_value* value, int depth, void* paste)
{
        int j;

        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
                        printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1, paste);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
                        printf("int: %10" PRId64 "\n", value->u.integer);
                        break;
                case json_double:
                        printf("double: %f\n", value->u.dbl);
                        break;
                case json_string:
                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}