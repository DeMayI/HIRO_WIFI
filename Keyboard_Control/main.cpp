#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <cstdio>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>   // Required to change socket to non-blocking
#include <cerrno>    // Required to check connection progress errors

#define KEY_SEEN 1
#define KEY_DOWN 2

enum ProgramState {
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_FAILED
};

int main(int argc, char** argv){

    
    if(!al_init()){
        printf("Failed to initialize Allegro\n");
        return 1;
    }
    if(!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    if(!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if(!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    if(!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    if(!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1){
        fprintf(stderr, "Failed to create socket!\n");
        return 1;
    }
    struct addrinfo* server_addr_list = NULL;
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    char ip_address[16];
    strcpy(ip_address, "192.168.1.168");
    if(argc > 1) {
        strcpy(ip_address, argv[1]);
    }
    //Creates a linked list containing all the open TCP server sockets on the local device listening on the port specified in args
    int info_result = getaddrinfo(
        ip_address,
        "23",
        &hints,
        &server_addr_list
    );
    if(info_result != 0){
        fprintf(stderr, "Error on getaddrinfo!\n");
        return 1;
    }
    int connect_result = -1;
    struct addrinfo* itr = server_addr_list;
    //Iterates through the linked list until it finds a server address it can connect to
    while(itr != NULL && connect_result == -1){
        connect_result = connect(
            socket_fd,
            itr->ai_addr,
            itr->ai_addrlen
        );
        itr = itr->ai_next;
    }
    if(connect_result == -1){
        fprintf(stderr, "Error on connect!\n");
        return 1;
    }
    unsigned char keys[ALLEGRO_KEY_MAX];
    memset(keys, 0, sizeof(keys));
    
    al_start_timer(timer);
    char last_command = 'n';
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                

                
                redraw = true;
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    keys[i] &= ~KEY_SEEN;
                break;


            case ALLEGRO_EVENT_KEY_DOWN:
                keys[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    done = true;
                }
                // SEND DIRECTION EXACTLY ONCE UPON KEY PRESS
                if (socket_fd != -1) {
                    if (event.keyboard.keycode == ALLEGRO_KEY_W)      {send(socket_fd, "w", 1, MSG_NOSIGNAL); last_command = 'w';}
                    else if (event.keyboard.keycode == ALLEGRO_KEY_A) {send(socket_fd, "a", 1, MSG_NOSIGNAL); last_command = 'a';}
                    else if (event.keyboard.keycode == ALLEGRO_KEY_S) {send(socket_fd, "s", 1, MSG_NOSIGNAL); last_command = 's';}
                    else if (event.keyboard.keycode == ALLEGRO_KEY_D) {send(socket_fd, "d", 1, MSG_NOSIGNAL); last_command = 'd';}  
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                keys[event.keyboard.keycode] &= ~KEY_DOWN;

                // SEND NEUTRAL ('n') EXACTLY ONCE UPON KEY RELEASE
                if (socket_fd != -1) {
                    int k = event.keyboard.keycode;
                    if (k == ALLEGRO_KEY_W || k == ALLEGRO_KEY_A || k == ALLEGRO_KEY_S || k == ALLEGRO_KEY_D) {
                        send(socket_fd, "n", 1, MSG_NOSIGNAL);
                        last_command = 'n';
                    }
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(15, 15, 25)); // Dark theme
            
            
           
            
            al_draw_text(font, al_map_rgb(0, 255, 0), 20, 45, 0, "Connected to HIRO! Press WASD to control the robot.");
            al_draw_textf(font, al_map_rgb(0, 255, 0), 20, 70, 0, "Last command sent: %c", last_command);
            
            
            al_flip_display();
            redraw = false;
        }
    }

    if(socket_fd != -1) close(socket_fd);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
