#include <gtk/gtk.h>

static void destroy(GtkWidget *wid, gpointer data){
	gtk_main_quit();
} 

static gboolean delete_event(GtkWidget *wid, GdkEvent *event, gpointer data){
	return FALSE;
}


GtkWidget *output;
gint boardMatrix[9] = {0};
gint playerTime = 0;
GtkWidget *button_array[9];
GtkWidget *button_board;
gboolean gameOver = FALSE;

void resetGame(GtkWidget *btn, gpointer data){
	int i;
	playerTime = 0;
	
	for( i = 0; i < 9; i++){
		boardMatrix[i] = 0;
		gtk_button_set_label(GTK_BUTTON(button_array[i]), "");
	}
	gtk_label_set_text(GTK_LABEL(output), "Player one start play");
}

gint analyseBoard( gint a[], gint n, gint key){
	
	if( (a[0] == key & a[1] == key && a[2] == key) || (a[3] == key && a[4] == key && a[5] == key) || (a[6] == key && a[7] == key && a[8] == key) || \
	    (a[0] == key && a[4] == key && a[8] == key) || (a[0] == key && a[3] == key && a[6] == key) || (a[1] == key && a[4] == key && a[7] == key) || \
	    (a[2] == key && a[5] == key && a[8] == key) || (a[6] == key && a[4] == key && a[2] == key) )
	    
		return key;	
	else
		return 0;
}

void record_move(GtkWidget *button, gpointer data){
	
	gint *numPtr = (gint *) data;
	gint num = *numPtr;
	
	
	if( gameOver ){
		resetGame(NULL, NULL);
		gameOver = FALSE;
	}
	else if( boardMatrix[num - 1] != 0){
		//Not a valid play
		if(playerTime % 2 == 0)
			gtk_label_set_label(GTK_LABEL(output), "   Invalid move, spot occupied, Player one play again");
		else
			gtk_label_set_label(GTK_LABEL(output), "   Invalid move, spot occupied, Player two play again");

	}
	else{ //Move was valid
	
		if(playerTime % 2 == 0){
			boardMatrix[num - 1] = 1;
			gtk_button_set_label(GTK_BUTTON(button), "X");
		}
		else{
			boardMatrix[num - 1] = 2;
			gtk_button_set_label(GTK_BUTTON(button), "O");
		}
		
		gint gameWon1 = analyseBoard(boardMatrix, 9, 1);
		gint gameWon2 = analyseBoard(boardMatrix, 9, 2);
		
		if( gameWon1 ){ 
			gtk_label_set_text(GTK_LABEL(output), "  Game Over\nPlayer one won!!");
			gameOver = TRUE;
		}
		else if( gameWon2){
			gtk_label_set_text(GTK_LABEL(output), "  Game Over\nPlayer two won!!");
			gameOver = TRUE;
		}
		else if(playerTime == 8)
			gtk_label_set_text(GTK_LABEL(output), "  Game Over\nNobody won");
		else if(playerTime < 8){
		
			if(playerTime % 2 == 0)
				gtk_label_set_text(GTK_LABEL(output), "Player two's time to play");
			else
				gtk_label_set_text(GTK_LABEL(output), "Player one's time to play");
			
			playerTime++;
		}
	}
}

int main(gint argc, char *argv[]){

	gtk_init(&argc, &argv);
	GtkWidget *window, *game_board, *button, *rBtn;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 400, 500);
	gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe game");
	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
	g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	button_board = gtk_table_new(3, 3, TRUE);
	gtk_widget_set_size_request(button_board, 800, 500);
	game_board = gtk_table_new(7, 3, TRUE);
	
	gint i = 0, j = 0, counter = 1;
	gint one = 1, two = 2, three = 3, four = 4, five = 5, six = 6, seven = 7, eight = 8, nine = 9;
	
	for( i = 0; i < 3; i++){
		for( j = 0; j < 3; j++){
			button_array[counter -1] = gtk_button_new_with_label("");
			
			//Assigns various callbacks to their specific buttons
			switch(counter){
				case 1:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &one);
					break;
				case 2:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &two);
					break;
				case 3:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &three);
					break;
				case 4:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &four);
					break;
				case 5:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &five);
					break;
				case 6:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &six);
					break;
				case 7:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &seven);
					break;
				case 8:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &eight);
					break;
				case 9:
					g_signal_connect(button_array[counter -1], "clicked", G_CALLBACK(record_move), &nine);
					break;
			}
			
			gtk_table_attach_defaults(GTK_TABLE(button_board), button_array[counter -1], j, j+1, i, i+1);
			gtk_widget_show_all(button_board);
			counter++;
		}
	}
	
	gtk_table_attach_defaults(GTK_TABLE(game_board), button_board, 0, 3, 0, 5);
	output = gtk_label_new("Player one start play");
	gtk_table_attach_defaults(GTK_TABLE(game_board), output, 0, 3, 5, 6);
	
	rBtn = gtk_button_new_with_mnemonic("_Reset game");
	g_signal_connect(rBtn, "clicked", G_CALLBACK(resetGame), NULL);
	gtk_table_attach_defaults(GTK_TABLE(game_board), rBtn, 0, 3, 6, 7);
	
	gtk_container_add(GTK_CONTAINER(window), game_board);
	gtk_widget_show_all(window);
	
	gtk_main();

}
