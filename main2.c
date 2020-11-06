#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <gtk/gtk.h>

#define USER2 2 
#define USER1 1 
#define NotReady -1 

GtkBuilder *builder;
GObject *window;
GObject *button;
GError *error = NULL;
int i = 0;

struct memory { 
	char buff[100];
	int status, pid1, pid2; 
}; 

struct memory* shmptr; 

// handler function to print message received from user1 
void handler(int signum) 
{ 
	// if signum is SIGUSR2, then user 2 is receiving a message from user1 
	if (signum == SIGUSR2) { 
		g_print("Received From User1: "); 
		puts(shmptr->buff);

        GtkWidget *label1;
        GObject *displayMessages;
        displayMessages = gtk_builder_get_object(builder, "fixedLayout");
        const gchar *user = "[USER]\t";
        const gchar *newUserMessage = g_strconcat(user, shmptr->buff, NULL);

        label1 = gtk_label_new (NULL);
        gtk_label_set_text((GtkLabel *)label1, newUserMessage); // insert the message
        gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
        g_object_set (label1, "margin-left", 10, NULL);
        g_object_set (label1, "margin-top", 10+i, NULL);
        const static GdkRGBA rgba_color = {.red = 0.0, .green = 1.0, .blue = 0.0, .alpha = 1.0};
        gtk_widget_override_color(label1, GTK_STATE_FLAG_NORMAL, &rgba_color);
        gtk_container_add (GTK_CONTAINER (displayMessages), label1);
        gtk_widget_show_all(label1);
        i=i+30;
	} 
} 

static void sendMessage (GtkWidget *widget, gpointer data)
{
    GtkEntry *message;
    GtkWidget *label, *label1;
    GObject *displayMessages;

    message = (GtkEntry *)gtk_builder_get_object (builder, "textInput");
    // const gchar *text = gtk_entry_get_text(message);
    // puts(text);

    sprintf(shmptr->buff, "%8s\n", gtk_entry_get_text(message));
    puts(shmptr->buff);

    displayMessages = gtk_builder_get_object(builder, "fixedLayout");
    
    const gchar *me = "[ME]\t";
    const gchar *newMessage = g_strconcat(me, gtk_entry_get_text(message), NULL);

    label = gtk_label_new (NULL);
    // gtk_label_set_text((GtkLabel *)label, gtk_entry_get_text(message));
    gtk_label_set_text((GtkLabel *)label, newMessage);

    gtk_entry_set_text(message, "");
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    g_object_set (label, "margin-left", 10, NULL);
    g_object_set (label, "margin-top", 10+i, NULL);


    gtk_container_add (GTK_CONTAINER (displayMessages), label);

    gtk_label_set_max_width_chars (GTK_LABEL (label), 100);
    gtk_widget_show_all(label);
    i=i+30;

    // fgets(shmptr->buff, 100, stdin);
    shmptr->status = USER1; 

    // sending the message to user2 using kill function 
    kill(shmptr->pid1, SIGUSR1);
    
    // while (shmptr->status != USER1) 
    //     continue; 
    // sleep(1); 
}

int main (int   argc, char *argv[])
{ 
	int pid = getpid(); 
	int shmid; 

    gtk_init (&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    button = gtk_builder_get_object (builder, "SendButton");
    g_signal_connect (button, "clicked", G_CALLBACK (sendMessage), NULL);


	int key = 1011; 
	shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666); 
	shmptr = (struct memory*)shmat(shmid, NULL, 0); 

	shmptr->pid2 = pid; 
	shmptr->status = NotReady; 

	// calling the signal function using signal type SIGUSR2 
	signal(SIGUSR2, handler); 

    gtk_main ();
    g_print("End");

	// while (1) { 
	// 	sleep(1); 

	// 	g_print("User2: "); 
	// 	// fgets(shmptr->buff, 100, stdin); 
	// 	shmptr->status = USER1; 

	// 	// sending the message to user1 using kill function 
	// 	kill(shmptr->pid1, SIGUSR1); 

	// 	while (shmptr->status == USER1) 
	// 		continue; 
	// } 
	shmdt((void*)shmptr); 
	return 0; 
} 
