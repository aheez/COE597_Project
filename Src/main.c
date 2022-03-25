#include "main.h"

static ALLEGRO_DISPLAY *display1;

static void redraw(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2) {

	al_set_target_backbuffer(display1);
	al_clear_to_color(color1);
	al_flip_display();

}

int main(int argc, char **argv){
	printf("Version: %s\n",ALLEGRO_VERSION_STR);

	// Initialize allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	// initialize the keyboard
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}
	// intialize the mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}
	// Initialize the display
	display1 = al_create_display(640, 480);
	if(!display1) {
		al_destroy_display(display1);
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);

	// mouse state
	ALLEGRO_MOUSE_STATE mst0;
	ALLEGRO_MOUSE_STATE mst;
	// keyboard state
	ALLEGRO_KEYBOARD_STATE kst;

	memset(&mst0, 0, sizeof(mst0));

	while (1)
	{
		al_get_mouse_state(&mst);

		if (mst.display != mst0.display || mst.x != mst0.x || mst.y != mst0.y)
		{
			if (mst.display == NULL)
				printf("Mouse is not on display\n");
			else if (mst.display == display1)
				printf("Mouse is on display\n");
			else
				printf("Uknown Display");
			
			mst0 = mst;
		}
		
		if (mst.display == display1)
		{
			redraw(red, black);
		}
		else
		{
			redraw(white, black);
		}

		al_rest(0.1);

		al_get_keyboard_state(&kst);
		if (al_key_down(&kst, ALLEGRO_KEY_ESCAPE))
			break;
	}

	// allegro distroy and exit
	al_destroy_display(display1);
	al_uninstall_mouse();
	al_uninstall_keyboard();
	
	return 0;
}