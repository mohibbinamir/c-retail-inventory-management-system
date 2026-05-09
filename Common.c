#include "Common.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(const char* prompt) {
    int value;
    char input[MAX_INPUT];
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &value) == 1) return value;
        printf("Invalid input. Please enter an integer.\n");
    }
}

float get_valid_float(const char* prompt) {
    float value;
    char input[MAX_INPUT];
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%f", &value) == 1) return value;
        printf("Invalid input. Please enter a number.\n");
    }
}

void get_string_input(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int is_valid_id(const char* id) {
    for (size_t i = 0; i < strlen(id); i++) {
        if (!isalnum(id[i]) && id[i] != '_') return 0;
    }
    return 1;
}

void pause_screen() {
    printf("\nPress ENTER to continue...");
    clear_input_buffer();
}
