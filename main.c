/*
 * Assessment 03 for PHY2027
 * Author: Guy Mangan
 * Date: 26/11/24
*/

/*
 * Program description: A programme that takes user inputs for the parameters surrounding a random walk in one, two or three dimensions. The two and three dimensional walks can be self avoiding. Results are outputted to text files, average distance and average squared distance for each set of walks is printed to the console.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define DELTA_X  1
#define DELTA_t  1
#define NUM_RUNS 100000
// Prototyping Functions

int x_coordinate(float p);

int y_coordinate(float p);

int z_coordinate(float p);

float p_calculator(float diffusion_constant);

// Main Function

int main(void)
{
    srand((unsigned int)time(0)); //seeding random number generation
    
    // Diffusion Constant Error Checks
    
    float diffusion_constant;
    printf("What is the Diffusion Constant of this simulation of a random walk? (Max. 0.5)\n");
    scanf("%f",&diffusion_constant);
    if (diffusion_constant<=0)
    {
        printf("You cannot have a diffusion constant of zero or below in this simulation");
        return 1;
    }
    if (diffusion_constant>0.5) //Because 1 - 2p must be positive or zero, so max p can be is 0.5
    {
        printf("You cannot have a diffusion constant of one or above in this simulation");
        return 1;
    }
    
    //Dimensions Error Checks
    
     int dimensions;
     printf("How many dimensions would you like to run this simulation of a random walk in?\n");
     scanf("%d",&dimensions);
     if (dimensions<1)
     {
         printf("You cannot have less than one dimension in this simulation");
         return 1;
     }
     if (dimensions>3)
     {
         printf("You cannot have more than three dimension in this simulation");
         return 1;
     }
    
    // Number of Steps Error Checks
    
    int num_steps = 0;
    int ask_num_steps; //done to ensure that num_steps is assigned to the correct value. Assigning num_steps directly in the scanf() function was producing unintended behaviors, the value 111 runs seemed to be hardcoded.
     printf("How many steps would you like to do per walk in this simulation? (max: 1000)\n");
     scanf("%d",&ask_num_steps);
     num_steps = ask_num_steps;
     if (num_steps<1)
     {
         printf("You cannot have less than one run in this simulation");
         return 1;
     }
     if (num_steps>1000)
     {
         printf("You cannot have more than 1000 steps in each run of this simulation");
         return 1;
     }
     
    //Self-Avoiding Check
    
    char self_avoiding[4];
    if (dimensions == 2 || dimensions == 3)
    {
         printf("Is this random walk self-avoiding? (Only available for 2 and 3 dimensional simulations)\n");
         scanf("%s",self_avoiding);

         if (strcmp(self_avoiding,"Yes") != 0 && strcmp(self_avoiding,"No") != 0 && strcmp(self_avoiding,"no") != 0 && strcmp(self_avoiding,"yes") != 0 )
         {
             printf("That is not a valid answer to this question.");
             return 1;
         }
    }

    // Calculating Probability of +dx, -dx or 0 being added
    float p = p_calculator(diffusion_constant);
    printf("In this walk, the probability of adding dx on each step is %0.2f\n",p);
    printf("In this walk, the probability of subtracting dx on each step is %0.2f\n",p);
    printf("In this walk, the probability of not moving on each step is %0.2f\n",(1-2*p));
    
    // Main Calculation Loops
    
    if (dimensions == 1)
    {
        double sum_absolute_x = 0;
        double sum_x_squared = 0;
        FILE* final_positions_file = fopen("/Users/guy/Desktop/PHY2027 Final Project/final_positions.txt", "w");
        for (int l = 0; l < NUM_RUNS; l++)
        {
            int x_position = 0;
            int** all_positions = malloc(num_steps * sizeof(int*));//Allocate memory to array
            for (int i = 0; i < num_steps; i++)
            {
                all_positions[i] = malloc(sizeof(int)); // Allocate memory for each position
                x_position = x_position + x_coordinate(p);
                *all_positions[i] = x_position; //saving new x position
            }
            int overall_position = *all_positions[num_steps - 1]; // final x coordinate
            fprintf(final_positions_file, "%d\n", overall_position); //printing to file of final positions
            
            // Prints all positions of the first run to a .txt file to be plotted
            if (l == 0)
            {
                FILE* file = fopen("/Users/guy/Desktop/PHY2027 Final Project/positions.txt", "w");
                for (int j = 0; j < num_steps; j++)
                {
                    fprintf(file, "%d\n", *all_positions[j]);
                }
                fclose(file);
            }
            //freeing memory
            for (int j = 0; j < num_steps; j++)
            {
                free(all_positions[j]);
            }
            free(all_positions);
            // Calculating sum of absolute x and x-squared position
            sum_absolute_x = sum_absolute_x + sqrt(overall_position*overall_position);
            sum_x_squared = sum_x_squared + (overall_position*overall_position);
        }
        // Calculating average of absolute x and x-squared position
        double avg_absolute_x = sum_absolute_x/NUM_RUNS;
        double avg_x_squared = sum_x_squared/NUM_RUNS;
        
        printf("Average of absolute x: %.2f\n", avg_absolute_x);
        printf("Average of x squared: %.2f\n", avg_x_squared);
        
        fclose(final_positions_file);//closes final positions file
        return 0;
    }
    
    if (dimensions == 2)
    {
        //Initializing variables
        double sum_absolute_r = 0;
        double sum_r_squared = 0;
        // Open final positions file
        FILE* final_positions_file = fopen("/Users/guy/Desktop/PHY2027 Final Project/final_positions.txt", "w");
        for (int l = 0; l < NUM_RUNS; l++)
        {
            int x_position = 0;
            int y_position = 0;
            int** used_pairs = malloc(num_steps * sizeof(int*));//Allocating memory to arrays
            int** all_positions = malloc(num_steps * sizeof(int*));
            for (int i = 0; i < num_steps; i++)
            {
                //self avoiding check
                if (strcmp(self_avoiding,"Yes") == 0 || strcmp(self_avoiding,"yes") == 0)
                {
                    //flag for used positions
                    int used = 0;
                    while (used == 0)
                    {
                        x_position = x_position + x_coordinate(p);
                        y_position = y_position + y_coordinate(p);
                        
                        used = 1;
                        
                        used_pairs[i] = malloc(2 * sizeof(int));
                        used_pairs[i][0] = x_position;
                        used_pairs[i][1] = y_position;
                        //testing position
                        for (int j = 1; j < i; j++)
                        {
                            if (used_pairs[j][0] == x_position && used_pairs[j][1] == y_position)
                            {
                                used = 0;//resets while loop
                                free(used_pairs[i]);//Frees position if it is not valid
                                break;//breaks loop
                            }
                        }
                    }
                    //Adds valid position to list of all visited positions
                    all_positions[i] = malloc(2 * sizeof(int));
                    all_positions[i][0] = x_position;
                    all_positions[i][1] = y_position;
                }
                //If walk is not self-avoiding
                else
                {
                    x_position = x_position + x_coordinate(p);
                    y_position = y_position + y_coordinate(p);
                
                    all_positions[i] = malloc(2 * sizeof(int));
                    all_positions[i][0] = x_position;
                    all_positions[i][1] = y_position;
                }
            }
            //prints to the file of final positions
            fprintf(final_positions_file,"%d %d \n",all_positions[num_steps - 1][0],all_positions[num_steps - 1][1]);
            // Prints all positions of the first run to a .txt file to be plotted
            if (l==0)
            {
                FILE* file = fopen("/Users/guy/Desktop/PHY2027 Final Project/positions.txt", "w");
                fprintf(file, "%d %d\n", 0,0); //initial position (origin)
                for (int j = 0; j < num_steps; j++)
                {
                    fprintf(file, "%d %d\n", all_positions[j][0], all_positions[j][1]);
                }
                fclose(file);
            }
            //Calculating sum of absolute r and r-squared position
            sum_absolute_r = sum_absolute_r + sqrt((all_positions[num_steps - 1][0]*all_positions[num_steps - 1][0])+(all_positions[num_steps - 1][1]*all_positions[num_steps - 1][1]));
            sum_r_squared = sum_r_squared + (all_positions[num_steps - 1][0]*all_positions[num_steps - 1][0])+(all_positions[num_steps - 1][1]*all_positions[num_steps - 1][1]);
            
            for (int j = 0; j < num_steps; j++)
            {
                free(all_positions[j]); //freeing each position in the arrays
                free(used_pairs[j]);
            }
            free(all_positions); //freeing the arrays
            free(used_pairs);
            
        }
        //Calculating average of absolute r and r-squared position
        double avg_absolute_r = sum_absolute_r/NUM_RUNS;
        double avg_r_squared = sum_r_squared/NUM_RUNS;
        
        printf("Average of absolute r: %0.2lf\n", avg_absolute_r);
        printf("Average of r squared: %0.2lf\n", avg_r_squared);
        
        fclose(final_positions_file);//closes final positions file
        return 0;
    }
    
    if (dimensions == 3)
    {
        //initialising variables
        double sum_absolute_r = 0;
        double sum_r_squared = 0;
        //opening final positions file
        FILE* final_positions_file = fopen("/Users/guy/Desktop/PHY2027 Final Project/final_positions.txt", "w");
        for (int l = 0; l < NUM_RUNS; l++)
        {
            int x_position = 0;
            int y_position = 0;
            int z_position = 0;
            int** used_pairs = malloc(num_steps * sizeof(int*));
            int** all_positions = malloc(num_steps * sizeof(int*));
            for (int i = 0; i < num_steps; i++)
            {
                //self avoiding check
                if (strcmp(self_avoiding,"Yes") == 0 || strcmp(self_avoiding,"yes") == 0)
                {
                    //flag for used positions
                    int used = 0;
                    while (used == 0)
                    {
                        x_position = x_position + x_coordinate(p);
                        y_position = y_position + y_coordinate(p);
                        z_position = z_position + z_coordinate(p);
                        //assume position is valid initially
                        used = 1;
                        //allocate memory to position to be tested
                        used_pairs[i] = malloc(3 * sizeof(int));
                        used_pairs[i][0] = x_position;
                        used_pairs[i][1] = y_position;
                        used_pairs[i][2] = z_position;
                        for (int j = 1; j < i; j++)
                        {   //testing position
                            if (used_pairs[j][0] == x_position && used_pairs[j][1] == y_position && used_pairs[j][2] == z_position)
                            {
                                used = 0;
                                free(used_pairs[i]);
                                break;
                            }
                        }
                    }//if it is valid, it is added to array of visited positions
                    all_positions[i] = malloc(3 * sizeof(int));
                    all_positions[i][0] = x_position;
                    all_positions[i][1] = y_position;
                    all_positions[i][2] = z_position;
                }
                //if the walk is not self avoiding
                else
                {
                    x_position = x_position + x_coordinate(p);
                    y_position = y_position + y_coordinate(p);
                    z_position = z_position + z_coordinate(p);
                    
                    all_positions[i] = malloc(3 * sizeof(int));
                    all_positions[i][0] = x_position;
                    all_positions[i][1] = y_position;
                    all_positions[i][2] = z_position;
                }
            }
            //writes final postions to file
            fprintf(final_positions_file,"%d %d %d \n",all_positions[num_steps - 1][0],all_positions[num_steps - 1][1],all_positions[num_steps - 1][2]);
            
            //writes every position of the first run to a file to be plotted
            if (l==0)
            {
                FILE* file = fopen("/Users/guy/Desktop/PHY2027 Final Project/positions.txt", "w");
                fprintf(file, "%d %d %d\n", 0,0,0); //initial position (origin)
                for (int j = 0; j < num_steps; j++)
                {
                    fprintf(file, "%d %d %d\n", all_positions[j][0], all_positions[j][1], all_positions[j][2]);
                }
                fclose(file);
            }
            //Calculating sum of absolute r and r-squared position
            sum_absolute_r = sum_absolute_r + sqrt((all_positions[num_steps - 1][0]*all_positions[num_steps - 1][0])+(all_positions[num_steps - 1][1]*all_positions[num_steps - 1][1])+(all_positions[num_steps - 1][2]*all_positions[num_steps - 1][2]));
            
            sum_r_squared = sum_r_squared +(all_positions[num_steps - 1][0]*all_positions[num_steps - 1][0])+(all_positions[num_steps - 1][1]*all_positions[num_steps - 1][1])+(all_positions[num_steps - 1][2]*all_positions[num_steps - 1][2]);
            
            for (int j = 0; j < num_steps; j++)
            {
                free(all_positions[j]);//freeing each position in the arrays
                free(used_pairs[j]);
            }
            free(all_positions);//freeing the arrays
            free(used_pairs);
        }
        //Calculating average of absolute r and r-squared position
        double avg_absolute_r = sum_absolute_r/NUM_RUNS;
        double avg_r_squared = sum_r_squared/NUM_RUNS;
        
        printf("Average of absolute r: %0.2lf\n", avg_absolute_r);
        printf("Average of r squared: %0.2lf\n", avg_r_squared);
        
        fclose(final_positions_file);//closes final positions file
        return 0;
    }
}


    // Function Definitions
float p_calculator(float diffusion_constant)
{
    float p = diffusion_constant*DELTA_t/(DELTA_X*DELTA_X);
    return p;
}


int x_coordinate(float p)
{
    float random_num = ((float)rand()/(float)RAND_MAX);
    if (random_num<(1-2*p))
    {
        return 0;
    }
    else if (random_num<(1-p)) // Will only trigger if first "if" is not true, so a lower bound is not neccesary
    {                          //change the contents of the if statement to make the walk asymetrical
        return DELTA_X;
    }
    else //remainder after 1-p
    {
        return -DELTA_X;
    }
}

int y_coordinate(float p)
{
    float random_num = ((float)rand()/(float)RAND_MAX);
    if (random_num<(1-2*p))
    {
        return 0;
    }
    else if (random_num<(1-p)) // Will only trigger if first "if" is not true, so a lower bound is not neccesary
    {                          //change the contents of the if statement to make the walk asymetrical
        return DELTA_X;
    }
    else //remainder after 1-p
    {
        return -DELTA_X;
    }
}

int z_coordinate(float p)
{
    float random_num = ((float)rand()/(float)RAND_MAX);
    if (random_num<(1-2*p))
    {
        return 0;
    }
    else if (random_num<(1-p)) // Will only trigger if first "if" is not true, so a lower bound is not neccesary
    {                          //change the contents of the if statement to make the walk asymetrical
        return DELTA_X;
    }
    else //remainder after 1-p
    {
        return -DELTA_X;
    }
}
