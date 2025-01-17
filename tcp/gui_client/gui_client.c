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

#include "utils/utils.h"
#include <stdio.h>
#include <string.h>

// gcc utils/utils.c gui_client.c -o test -Igooey/include -Igooey/internal -L./gooey/lib -Wl,-rpath,\$ORIGIN/gooey/lib  -lGooeyGUI -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined

// GOOEY
GooeyWindow win, result_window, auth_window;
GooeyWindow msgBox_auth_success, msgbox_auth_failure;
GooeyLayout *layout;
GooeyTextbox *inputBox;
GooeyList *list, *result_list;
GooeyTextbox *username_textbox;
GooeyTextbox *password_textbox;
char *date_result = "";

// SOCKET
int sock;
msg message = {0};

void get_user_choice(int op)
{
    switch (op + 1)
    {
    case 1:
        // DATE
        message.op = 1;
        memset(message.buff, 0, 4096);
        safe_send(sock, -1, &message);
        safe_rcv(sock, -1, &message);
        GooeyWindow_MakeVisible(&result_window, true);
        GooeyList_ClearItems(result_list);
        message.buff[strcspn(message.buff, "\n")] = '\0';
        GooeyList_AddItem(result_list, "", message.buff);
        GooeyWindow_Redraw(&result_window);

        break;

    case 2:
        // LS
        message.op = 2;
        memset(message.buff, 0, 4096);
        strcat(message.buff, ".");
        safe_send(sock, -1, &message);
        safe_rcv(sock, -1, &message);
        GooeyWindow_MakeVisible(&result_window, true);
        GooeyList_ClearItems(result_list);
        GooeyList_ShowSeparator(result_list, true);

        const char *start = message.buff;
        const char *end;

        while ((end = strchr(start, '\n')) != NULL)
        {
            char line[256];
            size_t length = end - start;

            if (length >= sizeof(line))
            {
                length = sizeof(line) - 1;
            }

            strncpy(line, start, length);
            line[length] = '\0';
            GooeyList_AddItem(result_list, "", line);
            start = end + 1;
        }

        if (*start != '\0')
        {
            GooeyList_AddItem(result_list, "", start);
        }
        GooeyWindow_Redraw(&result_window);
        break;

    case 3:

        /* Implement this when Logger widget is available in lib. */
        // CAT
        message.op = 3;
        memset(message.buff, 0, 4096);
        strcat(message.buff, "common.h");
        safe_send(sock, -1, &message);
        safe_rcv(sock, -1, &message);
        GooeyWindow_MakeVisible(&result_window, true);
        GooeyList_ClearItems(result_list);
        GooeyList_ShowSeparator(result_list, false);
        const char *start_ = message.buff;
        const char *end_;

        while ((end_ = strchr(start_, '\n')) != NULL)
        {
            char line[256];
            size_t length = end_ - start_;

            if (length >= sizeof(line))
            {
                length = sizeof(line) - 1;
            }

            strncpy(line, start_, length);
            line[length] = '\0';
            GooeyList_AddItem(result_list, line, "");
            start_ = end_ + 1;
        }

        if (*start_ != '\0')
        {
            GooeyList_AddItem(result_list, start_, "");
        }
        GooeyWindow_Redraw(&result_window);
        break;

    case 4:
        // ELAPSED
        message.op = 4;
        memset(message.buff, 0, 4096);
        safe_send(sock, -1, &message);
        safe_rcv(sock, -1, &message);
        GooeyWindow_MakeVisible(&result_window, true);
        GooeyList_ClearItems(result_list);
        message.buff[strcspn(message.buff, "\n")] = '\0';
        GooeyList_AddItem(result_list, "", message.buff);
        GooeyWindow_Redraw(&result_window);
        break;

    default:
        break;
    }
}

void client_connect()
{
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PROXY_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock = safe_socket();
    safe_connect(sock, &server_addr);
}

void client_disconnect()
{
    // Send disconnection
    message.op = 5;
    safe_send(sock, -1, &message);
    close(sock);
}

void client_auth()
{
    char *username = (char *)GooeyTextbox_GetText(username_textbox);
    char *password = (char *)GooeyTextbox_GetText(password_textbox);

    // GooeyWindow_MakeVisible(&authWindow, false);
    char *args[2] = {username, password};
    message.op = -1; // auth op
    concat_args(message.buff, args, sizeof(args) / sizeof(args[0]));

    safe_send(sock, -1, &message);
    safe_rcv(sock, -1, &message);
    if (message.op == 0)
    {
        // Success
        GooeyMessageBox_Show(&msgBox_auth_success);
        GooeyWindow_MakeVisible(&auth_window, false);
        // List the operations
        GooeyList_AddItem(list, "Service: date", "This service returns the full current date.");
        GooeyList_AddItem(list, "Service: ls", "This service lists a directory's content.");
        GooeyList_AddItem(list, "Service: cat", "This service dumps a file's content.");
        GooeyList_AddItem(list, "Service: elapsed", "This service returns the elapsed time since client connection.");
        GooeyWindow_Redraw(&win); // Redraw window. (Bug to be fixed in lib. this shouldn't require redraw.)
    }
    else
    {
        GooeyMessageBox_Show(&msgbox_auth_failure);
    }
}

int main()
{
    Gooey_Init(GLFW);
    client_connect();
    win = GooeyWindow_Create("Sockets Client", 400, 300, true);
    auth_window = GooeyWindow_CreateChild("Authentication", 300, 260, true);
    result_window = GooeyWindow_CreateChild("Service Results", 400, 300, false);

    result_list = GooeyList_Add(&result_window, 0, 0, 390, 300, NULL);

    msgBox_auth_success = GooeyMessageBox_Create("Authenticated!", "You have been authenticated", MSGBOX_SUCCES, NULL);
    msgbox_auth_failure = GooeyMessageBox_Create("Error", "Cannot be authenticated, please retry.", MSGBOX_FAIL, NULL);

    GooeyLayout *auth_layout = GooeyLayout_Create(&auth_window, LAYOUT_VERTICAL, 10, 40, 280, 300);
    GooeyLabel *auth_label = GooeyLabel_Add(&auth_window, "Authenticate client", 0.5f, 40, 40);
    username_textbox = GooeyTextBox_Add(&auth_window, 0, 0, 40, 40, "Username", NULL);
    password_textbox = GooeyTextBox_Add(&auth_window, 0, 0, 40, 40, "Password", NULL);
    GooeyButton *auth_btn = GooeyButton_Add(&auth_window, "Connect", 0, 0, 40, 40, client_auth);
    GooeyButton_SetHighlight(auth_btn, true);
    GooeyLayout_AddChild(auth_layout, auth_label);
    GooeyLayout_AddChild(auth_layout, username_textbox);
    GooeyLayout_AddChild(auth_layout, password_textbox);
    GooeyLayout_AddChild(auth_layout, auth_btn);
    GooeyLayout_Build(auth_layout);
    GooeyWindow_MakeResizable(&auth_window, false);
    GooeyWindow_MakeResizable(&win, false);
    GooeyWindow_MakeResizable(&result_window, false);

    layout = GooeyLayout_Create(&win, LAYOUT_VERTICAL, 5, 5, 380, 290);

    list = GooeyList_Add(&win, 0, 0, 390, 300, get_user_choice);
    GooeyList_ShowSeparator(list, true);

    GooeyWindow_Run(5, &win, &auth_window, &msgBox_auth_success, &msgbox_auth_failure, &result_window);
    GooeyWindow_Cleanup(5, &win, &auth_window, &msgBox_auth_success, &msgbox_auth_failure, &result_window);

    client_disconnect();
    return 0;
}
