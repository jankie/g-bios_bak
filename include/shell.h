#pragma once

void set_curr_volume(char vol);
char get_curr_volume(void);

char get_home_volume(void);
int set_home_volume(char vol);

int shell(void);

int exec(int argc, char *argv[]);

int usage(void);
