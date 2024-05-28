#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 1000
#define MAX_CODE_LENGTH 10000

int main(int argc, char *argv[]) {
    FILE *file;
    char filename[MAX_FILENAME_LENGTH];
    char code[MAX_CODE_LENGTH];
    char input[MAX_CODE_LENGTH];
    char command;
    char compile_command[MAX_FILENAME_LENGTH + 10]; 
    int compile_status;
    int line_number;

   if (argc != 2) {
        printf("Enter the filename ");
        fgets(filename, MAX_FILENAME_LENGTH, stdin);
        filename[strcspn(filename, "\n")] = 0;

        file = fopen(filename, "r+");
        if (file == NULL) {
            file = fopen(filename, "w+");

        }
    } else {
        strcpy(filename, argv[1]);
        file = fopen(filename, "a+");
    }

    fread(code, sizeof(char), MAX_CODE_LENGTH, file);

    printf("MinEdit: A Minimalist CLI-Based C Program Editor\n");
    printf("Enter :c to change the line in the code\n");
    printf("Enter :!c to compile the program\n");
    printf("Enter :e to execute the program\n");
    printf("Enter :x to close the file\n");    
    printf("Enter :q to quit\n\n");

    int command_mode = 0; 
    while (1) {
        if (!command_mode) {
            printf("%s", code);
            fgets(input, MAX_CODE_LENGTH, stdin);
        }

        if (input[0] == ':' || command_mode) {
            if (!command_mode) {
                sscanf(input, ":%c", &command);
            } else {
                sscanf(input, "%c", &command);
            }
            switch (command) {
                case 'q': 
                    if (command_mode || (strlen(code) == 0 || strcmp(code, "\n") == 0)) {
                        fclose(file);
                        printf("Exiting MinEdit...\n");
                        return 0;
                    } else {
                        printf("Are you sure you want to quit without saving changes? (y/n): ");
                        char response;
                        scanf(" %c", &response);
                        if (response == 'y' || response == 'Y') {
                            fclose(file);
                            printf("Exiting MinEdit...\n");
                            return 0;
                        } else {
                            command_mode = 0;
                            continue;
                        }
                    }
                case 's': 
                    rewind(file);
                    fwrite(code, sizeof(char), strlen(code), file);
                    fflush(file);
                    printf("File saved successfully!\n");
                    break;
                case 'x': 
                    if (strlen(code) == 0 || strcmp(code, "\n") == 0) {
                        fclose(file);
                        printf("File closed.\n");
                        break;
                    } else {
                        printf("Are you sure you want to close without saving changes? (y/n): ");
                        char response;
                        scanf(" %c", &response);
                        if (response == 'y' || response == 'Y') {
                            fclose(file);
                            printf("File closed.\n");
                            return 0;
                        } else {
                            command_mode = 0; 
                            continue;
                        }
                    }
                case 'e': 
                    sprintf(compile_command, "./%s.out", filename);
                    system(compile_command);
                    break;
                case '!': 
                    sprintf(compile_command, "gcc %s -o %s.out", filename, filename);
                    compile_status = system(compile_command);
                    if (compile_status == 0) {
                        printf("Compilation successful! Executable created: %s.out\n", filename);
                    } else {
                        printf("Compilation failed!\n");
                    }
                    break;
                case 'c': 
                    printf("Enter the line number you want to change: ");
                    scanf("%d", &line_number);
                    getchar(); 
                    if (line_number >= 1 && line_number <= MAX_CODE_LENGTH) {
                        printf("Enter the new line: ");
                        fgets(input, MAX_CODE_LENGTH, stdin);
                        input[strcspn(input, "\n")] = 0; 
                        char *pos = strchr(code, '\n');
                        int current_line = 1;
                        while (pos != NULL && current_line < line_number) {
                            pos = strchr(pos + 1, '\n');
                            current_line++;
                        }
                        if (pos != NULL) {
                            fseek(file, pos - code + 1, SEEK_SET);
                            fputs(input, file);
                            printf("Line %d changed successfully!\n", line_number);
                        } else {
                            printf("Invalid Line");
                        }
                        code[0] = '\0';
                        fseek(file, 0, SEEK_SET); 
                        fread(code, sizeof(char), MAX_CODE_LENGTH, file); 
                    } else {
                        printf("Invalid line");
                    }
                    break;
                case '\n': 
                    break;
                default:
                    printf("Unknown command: %c\n", command);
            }
            command_mode = 0; 
        } else {
            strcat(code, input);
            command_mode = 0; 
        }
    }

    return 0;
}

