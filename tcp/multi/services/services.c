/*
 Copyright (c) 2025 Yassine Ahmed Ali

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "services.h"
static void __exec_cmd(char *cmd[], char *buff)
{
    pid_t pid;
    int p[2];

    if (pipe(p) < 0)
    {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        close(p[1]);
        if (execvp(cmd[0], cmd) == -1)
        {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        close(p[1]);
        ssize_t curr_read = read(p[0], buff, 4096 - 1);
        if (curr_read <= 0)
        {
            perror("read failed or no data");
        }
        else
        {
            buff[curr_read] = '\0';
        }
        close(p[0]);
        wait(NULL);
    }
}

bool authentification(const char *nom_utilisateur, const char *mdp)
{
    // Data pour simulation
    const char *sim_user = "sim";
    const char *sim_mdp = "sim";

    if (strcmp(nom_utilisateur, sim_user) == 0 && strcmp(mdp, sim_mdp) == 0)
    {
        return true;
    }
    return false;
}

void get_date_complete(char *time_str)
{
    char *args[] = {"date", "+%Y/%m/%d %H:%M:%S", NULL};
    __exec_cmd(args, time_str);
}

void get_liste_fichiers(char *dir_name, char *buff)
{

    char *args[] = {"ls", dir_name, NULL};
    __exec_cmd(args, buff);
}

void get_contenu_fichier(char *file_name, char *buff)
{
    char *args[] = {"cat", file_name, NULL};
    __exec_cmd(args, buff);
}

void get_time_taken(struct timespec *start_time, char *buff)
{
    struct timespec end_time = {0};
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double time_taken = (end_time.tv_sec - start_time->tv_sec) +
                        (end_time.tv_nsec - start_time->tv_nsec) / 1e9;

    if (buff)
    {
        memset(buff, 0, 4096);
        sprintf(buff, "%lfs", time_taken);
    }
}