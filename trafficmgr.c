#include "traffic.h"

void cleanexit() {
    q_delete('n');
    q_delete('s');
    q_delete('e');
    q_delete('w');
    q_shutdown;
    exit(EXIT_FAILURE);
}

void argerror() {
    printf("Correct Usage: ./trafficmgr <d1d2d3d4...>\n Where dn E [n,s,e,w] is the direction of the nth cart.\n");
    cleanexit();
}

/*
 *  Checks if the given string matches the defined synax for command line args
 *
 *  arg - string to check
 */
int check_match(char *arg) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    /* Compile regular expression */
    reti = regcomp(&regex, "^[nsew]*$", 0);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, arg, 0, NULL, 0);
    if (!reti) {
        /* match found */
        printf("Expression matched: %s\n", arg);
        return 1;
    } else if (reti == REG_NOMATCH) {
        /* no match found */
        printf("Expression not matched: %s\n", arg);
        return -1;
    } else {
        /* expression match failed */
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(-1);
    }

    /* Free compiled regular expression */
    regfree(&regex);
}

void init(char* arg) {
    int i;

    q_init;
    for(i = 0; arg[i] != '\0'; i++){
        switch (arg[i]) {

            case 'n':
                q_putCart('n');
                break;
            case 's':
                q_putCart('s');
                break;
            case 'e':
                q_putCart('e');
                break;
            case 'w':
                q_putCart('w');
                break;
        }
    }

}

int main(int argc, char** argv) {

    if (argc == 2 && check_match(argv[1]) > 0) {
        init(argv[1]);
        //q_print('n');
        //q_print('s');
        //q_print('e');
        //q_print('w');
    } else {
        argerror();
    }

    cleanexit();
    return 0;
}
