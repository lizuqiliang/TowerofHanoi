//Zuqi Li || ICS4U || Mr. Kronberg

//Headers-------------------------------------------------------------------------------------------------------------------------------
#include<stdio.h>
#include<allegro.h>
#include<iostream>
#include<fstream>
#include<string.h>
#include<math.h>
#include"stack.h" 
#include"stack.cpp"

using namespace std;

volatile long speed_counter = 0;
void increment_speed_counter(){speed_counter++;}

//Print function to output the Hanoi Towers---------------------------------------------------------------------------------------------------------------
void print(BITMAP* buffer, BITMAP* b, BITMAP* d, BITMAP *p, stack peg[], int size, int speed, int &step, int &calls){
     
    step++;                //Increment step counter to coutn the number of steps it takes to solve a Hanoi
    int h = size;          //Set height of disk
    int w = 2*h;           //set width of disk
	int counter = 0;       //declare counter to count for how long a frame shows on screen
	float length = 0;      //declare the length to temporarily store the number of disks of a peg
	
	//While loop that prints out the disks and pegs for an amount of time
	while(counter < speed){              //While counter is less than a number of time, display the frame onto screen
	    counter++;                       //Increment counter
		draw_sprite(buffer, b, 0, 0);    //Drawing the background
		float num;                       //Number to store the disk number
		stack temp;                      //Temporary stack used to display the disks of a given peg
		
		//For loop to draw the pegs and the disks on each peg-----------------------------------------------------------------------------------------------
		for(int i=0;i<3;i++){
			length = peg[i].getlength();                                   //Temporary store the number of disks on a given peg
			stretch_sprite(buffer, p, i*400+212+h, 300, 16, 300);          //Draw the peg with width 16 and height 300 by stretching the original sprite
			
			for(int j=0;j<length;j++)
				temp.push(peg[i].pop());                                   //Push the disks from the original stack into the temporary stack
				
			for(int j=0;j<length;j++){
				num=temp.pop();                                            //Determine the size of the disk popped out
				stretch_sprite(buffer, d, i*400+220-num*h, 600-j*h-h, w+num*w, h);     //Stretch the sprite to the appropriate size and draw it into buffer
				peg[i].push(num);                                          //Pushes the disk back into the original stack
			}
		}
		
		//Print out the number of steps and number of calls onto the screen-------------------------------------------------------------------
		textprintf_ex(buffer, font, 50, 50, makecol(0, 0, 0),-1, "Steps: %d", step);
		textprintf_ex(buffer, font, 50, 75, makecol(0, 0, 0),-1, "Calls: %d", calls);
		
		//Draw the buffer onto the screen
		blit(buffer, screen, 0,0,0,0,1280,600);  
		clear_bitmap(buffer);
	}
}

//Hanoi Function to carry out the algorithm-------------------------------------------------------------------------------------------------------------
void hanoi(BITMAP* buffer, BITMAP* back, BITMAP* disk, BITMAP *pegs, int n, int s, int d, int m, stack peg[], int size, int speed,int &step, int &calls){
    calls++;               //Increments calls to record the number of times this function is called
    
    //If there is only one disk
	if(n == 1){
		peg[d].push(peg[s].pop());                                          //Moving the disk from the starting position to the destination
		print(buffer,back,disk,pegs,peg,size ,speed, step, calls);          //Updates the frame on the screen
	}
	
	//If there are multiple disks
	else{
		hanoi(buffer,back,disk,pegs,n-1,s,m,d,peg,size, speed,step,calls);  //Calls the function to move a stack of disks from starting position to the intermediate/middle position
		peg[d].push(peg[s].pop());                                          //Moving the disk from the starting position to the destination
		print(buffer,back,disk,pegs,peg,size, speed, step,calls);           //Updates the frame on on the screen
		hanoi(buffer,back,disk,pegs,n-1,m,d,s,peg,size, speed,step,calls);  //Calls the function to move a stack of disks from the intermediate/middle position to the destination
	}
}

//Displays the opening logo animation
void display_title(){
     BITMAP *title = load_bitmap("Studio.bmp", NULL);                       //Loads Logo bitmap
     BITMAP *buffer = create_bitmap(1280,600);                              //Creates buffer bitmap
     int loop = 0;                                                          //loops the animation
     int counter = 250;                                                     //Declaring the counter for the fade in/out effect
     bool max = false;                                                      //declaring maximum brightness of logo
     
     //while fade in/out effect is not finished
     while(loop==0){
         while(speed_counter > 0){
             if(max)counter+=5;                //If maximum has been reached starts fading out
             else counter-=3;                  //If not starts fading in
             if(counter < 4)max = true;        //When coutner is less than 4, the max is reached
             if(counter>230 && max)loop = 1;   //If the max is reached and counter is greater than 230, the logo has faded out and it is time to loop out

             speed_counter--;
         }
         
         draw_lit_sprite(buffer,title, 1280/2-title->w/2, 600/2-title->h/2,counter);          //Draws sprite and uses the coutner variab,le to determine its brightness
         blit(buffer,screen,0,0,0,0,1280,600);                                                //Draws Buffer to screen
         clear_bitmap(buffer);                                                                //Clears Buffer
     }
}

//Function to display main menu------------------------------------------------------------------------------------------------------
void display_menu(int &s, int &n){
     //Load Bitmaps------------------------------------------------------------------------------------------------------------------
     BITMAP *menu = load_bitmap("menu.bmp", NULL);
     BITMAP *start = load_bitmap("start.bmp", NULL);
     BITMAP *blank = load_bitmap("blank.bmp", NULL);
     BITMAP *minus = load_bitmap("minus.bmp", NULL);
     BITMAP *add = load_bitmap("add.bmp", NULL);
     BITMAP *padd = load_bitmap("padd.bmp", NULL);
     BITMAP *pminus = load_bitmap("pminus.bmp", NULL);
     BITMAP *buffer = create_bitmap(1280,600);
     
     bool loop = false;                                             //Decides when to loop in/out of menu
     bool hold = false;                                             //Checking mouse button's keyhold
     int counter[4] = {90,90,90,90};                                //Counter for button rpess animation
     int hold_counter = 90;                                         //Counter for the  mosue keyhold
     
     while (!loop){
         draw_sprite(buffer, menu, 0, 0);                           //Draw the background
         draw_sprite(buffer, start, 640-start->w/2, 500);           //Draw the start button
         for(int i=0;i<2;i++){
             draw_sprite(buffer, blank, 640-start->w/2, 200+i*150);            //draw the middle button that displays the number
             draw_sprite(buffer, minus, 640-start->w/2-100, 200+i*150);        //draws the minus sign
             draw_sprite(buffer, add, 640-start->w/2+100, 200+i*150);          //draws the plus sign
         }
         
         //Prints the number of disks part
         textprintf_ex(buffer, font, 665-start->w, 170, makecol(0, 0, 0),-1, "Number of Disks");
         textprintf_ex(buffer, font, 690-start->w, 185, makecol(0, 0, 0),-1, "(1 - 100)");
         textprintf_ex(buffer, font, 717-start->w, 217, makecol(0, 0, 0),-1, "%d", n);
         
         //Prints the speed part
         textprintf_ex(buffer, font, 700-start->w, 320, makecol(0, 0, 0),-1, "Speed");
         textprintf_ex(buffer, font, 695-start->w, 335, makecol(0, 0, 0),-1, "(1 - 5)");
         textprintf_ex(buffer, font, 717-start->w, 367, makecol(0, 0, 0),-1, "%d", s);
         
         //Mouse click detection
         if((mouse_b & 1) && hold == false){
             hold_counter = 0;    //If mouse is clicked, hold counter becomes 0 and variable "hold" becomes true for 20 cycles
             
             if(mouse_x > 640-start->w/2-100 && mouse_x < 640+start->w/2-100 && mouse_y>200 && mouse_y<200+start->h){
                  if(n>1) n--;                    //Number of disk decreases
                  counter[0] = 0;                 //Button pressed sprite is drawn for 30 cycles
             }
             else if(mouse_x > 640-start->w/2+100 && mouse_x < 640+start->w/2+100 && mouse_y>200 && mouse_y<200+start->h){
                  if(n<100) n++;                  //Disks increases      
                  counter[1] = 0;                 //Button pressed sprite is drawn for 30 cycles
             }
             else if(mouse_x > 640-start->w/2-100 && mouse_x < 640+start->w/2-100 && mouse_y>350 && mouse_y<350+start->h){
                  if(s>1)s--;                     //Speed decreases
                  counter[2] = 0;                 //Button pressed sprite is drawn for 30 cycles
             }
             else if(mouse_x > 640-start->w/2+100 && mouse_x < 640+start->w/2+100 && mouse_y>350 && mouse_y<350+start->h){
                  if(s<5)s++;                     //speed increases
                  counter[3] = 0;                 //Button pressed sprite is drawn for 30 cycles
             }
             else if(mouse_x > 640-start->w/2 && mouse_x < 640+start->w/2 && mouse_y>500 && mouse_y<500+start->h){
                  loop = true;                    //If start is pressed, exit loop
             }
         }
         
         //If hold_coutner is less than 20 cycles, hold is true
         if (hold_counter < 20)hold = true;
         else hold = false;
         
         //Draw sprite if animation coutner is less than 35 cycles
         if (counter[0] < 30) draw_sprite(buffer, pminus, 640-start->w/2-100, 200);
         if (counter[1] < 30) draw_sprite(buffer, padd, 640-start->w/2+100, 200);
         if (counter[2] < 30) draw_sprite(buffer, pminus, 640-start->w/2-100, 350);
         if (counter[3] < 30) draw_sprite(buffer, padd, 640-start->w/2+100, 350);
         
         //Increment counters
         for(int i=0;i<4;i++)  counter[i]++;
         hold_counter++;
         
         show_mouse(buffer);                      //Draws mouse to buffer
         blit(buffer,screen,0,0,0,0,1280,600);    //Draws buffer to screen
         clear_bitmap(buffer);                    //Clears Buffer
     }
}

//End funciton after the hanoi is completely solved------------------------------------------------------------------------------
void end(int &s, int &n,int &step,int &calls, bool &loop, stack peg[]){
     show_mouse(screen);              //Draws mouse on screen
     
     //Loads Bitmaps
     BITMAP *exit =  load_bitmap("exit.bmp", NULL);      
	 BITMAP *restart =  load_bitmap("noexit.bmp", NULL);
	 
     bool decision = false;           //Declares decision
     
     s = 3;                           //Reset speed to 3
     n = 3;                           //Reset number of disks to 3
     step = -1;                       //reset number of steps took
     calls = 0;                       //reset number of calls took
     int length = 0;                  //declares a temporary length variable to store the number of disks on the peg
     
     //Loop used to clear all the disks on the destination peg---------------------------
     length = peg[2].getlength();
     for(int i=0;i<length;i++)
		peg[2].pop();
     
     //Draws the exit and restart button-----------------------------------
     draw_sprite(screen, exit, 680-100-exit->w, 150);
     draw_sprite(screen, restart, 680+100-exit->w, 150);
     
     //Loops as long player does not make a decision
     while(!decision){
         if((mouse_b & 1)){
             //If exit button is pressed, the program does not loop and ends
             if(mouse_x > 680-100-exit->w && mouse_x < 680-100+exit->w && mouse_y>150 && mouse_y<150+exit->h){
                 loop = false;
                 decision = true;
             }
             //If restart button is pressed, the program loops and starts from main menu
             else if(mouse_x > 680+100-exit->w && mouse_x < 680+100+exit->w && mouse_y>150 && mouse_y<150+exit->h){
                 loop = true;
                 decision = true;
             }
         }
     }
}

//Main---------------------------------------------------------------------------------------------------
int main(){
    
    //Initializes Allegro------------------------------------------------------------------------------
	allegro_init(); 
    install_keyboard();
    install_mouse();
    
    set_color_depth(desktop_color_depth());
   	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1280, 600,0,0);
   	
   	//Speed coutner used for the beginning fade in/out effect
	LOCK_VARIABLE(speed_counter);
 	LOCK_FUNCTION(increment_speed_counter);
   	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); 	
	
	//Set up light table for fade in/out effect
	COLOR_MAP global_light_table;
    set_trans_blender(0, 0, 0, 0);
	
    stack peg[3];              //The pegs
    int size = 3;              //Size of the disks
    int s = 3;                 //Speed
    int n = 3;                 //Number of disks
    int steps = -1;            //Steps took
    int calls = 0;             //number of calls to function
    bool loop = true;          //determines whether to loop the program
    
    //Loads bitmaps
   	BITMAP *buffer = create_bitmap(1280,600);
	BITMAP *back =  load_bitmap("back.bmp", NULL);
	BITMAP *disk =  load_bitmap("se7en.bmp", NULL);
	BITMAP *pegs =  load_bitmap("peg.bmp", NULL);
    
    //Displays the opening fade in/out animation
	display_title();
	
	//If loop is true, teh program will return back to here-------------------------------------------------------
	while (loop==true){
    	display_menu(s,n);              //Displays the main menu
    	
    	for(int i = n; i>0; i--){       //Prepares the numebr of disks in the first peg
    		peg[0].push(i);
    	}
    	
    	//Prepare the speed based on the user's input--------------------------------------------------------------
    	if(s == 5)s=1;
    	else if(s == 4)s=20;
    	else if(s == 3)s=40;
    	else if(s == 2)s=60;
    	else if(s == 1)s=80;
    	
    	//Based on the number of disks, choose the appropriate disk sizes to fit into the pegs---------------------------------
    	if(n>0 && n <= 5) size = 20;
    	else if(n>5 && n <= 10) size = 16;
    	else if(n>10 && n <= 15) size = 12;
    	else if(n>15 && n <=20) size = 8;
    	else if(n>20 && n <=45) size = 4;
    	else size = 2;
    	
    	print(buffer,back,disk,pegs,peg,size, s,steps,calls);             //Prints the first beginning frame where all the disks are in the first peg
    	hanoi(buffer,back,disk,pegs,n,0,2,1,peg,size, s,steps,calls);     //Begins algorithm and prints the results
    	end(s,n,steps,calls,loop,peg);                                    //Ending menu
    }
	return 0;
}
END_OF_MAIN()
