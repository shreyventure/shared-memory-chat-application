// gcc `pkg-config --cflags gtk+-3.0` -o a.out new.c `pkg-config --libs gtk+-3.0`

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GtkBuilder *builder;
GObject *window;
GObject *button;
GError *error = NULL;
int i = 0;

static void sendMessage (GtkWidget *widget, gpointer data)
{
  GtkEntry *message;
  GtkWidget *label, *label1;
  GObject *displayMessages;

  message = (GtkEntry *)gtk_builder_get_object (builder, "textInput");
  const gchar *text = gtk_entry_get_text(message);
  puts(text);

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

  gtk_label_set_max_width_chars (GTK_LABEL (label), 125);
  gtk_widget_show_all(label);
  i=i+30;

  // client

  const gchar *user = "[USER]\t";
  const gchar *newUserMessage = g_strconcat(user, "Hello", NULL);

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

int main (int   argc, char *argv[])
{

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

  gtk_main ();
  g_print("\n[INFO] Application was closed.\n");

  return 0;
}