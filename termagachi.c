#define _GNU_SOURCE     /* for asprintf */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define TERMAGACHI_LOCAL_FILE ".dontmindme"

typedef enum
{
    REGULAR = 0,
    HAPPY,
    HUNGRY,
    SLEEPY,
    PLAY,
    LOSE,
    WIN
} _MOOD;

void print_termagachi(_MOOD mood)
{
    char *msg = NULL;

    printf("\t  __\n");
    printf("\t (");

    switch(mood) {
        case HAPPY:
            printf("^^");
            msg = "Woo! Loving life!";
            break;
        case HUNGRY:
            printf("**");
            msg = "So...hungry...feed me more git commits....NOMNOMNOM";
            break;
        case SLEEPY:
            printf("><");
            msg = "Sshhhh! Nap time!";
            break;
        case PLAY:
            printf("^^");
            msg = "Yay a game! You're going DOWN.";
            break;
        case WIN:
            printf("^^");
            msg = "YAY! Welp, better luck next time, huh? Way to lose, suckaaahh.";
            break;
        case LOSE:
            printf("~~");
            msg = "Oh awesome..you win! Back to coding then for you...";
            break;
        default:
            printf("--");
            msg = "Sup, homie?";
            break;
    }

    printf(")\n");
    printf("\t \\||/\n");
    printf("\t  /\\\n");
    printf("\n");
    printf("___%s___\n", msg);
    printf("\n");
}

void print_help()
{
    printf("\n");
    printf("Welcome to Termagachi! Your very own terminal companion.\nTo feed her, she needs looottss of git commits. A Termagachi without food gets angry. She also likes to play and say hi.\n");
    printf("\n");
    printf("Usage options: termagachi [command]\n");
    printf("Commands:\n");
    printf("\tstatus\t->See how your Termagachi is doing\n");
    printf("\tsayhi\t->Say hi to your Termagachi!\n");
    printf("\tplay\t->Play with Termagachi!\n");
    printf("\n");
}

void setup()
{
    int num_commits;
    char input_path[80];
    FILE *fp = NULL;
    
    if( (fp = fopen(TERMAGACHI_LOCAL_FILE, "w")) == NULL)
    {
        printf("ERROR: %d: fp is null\n", __LINE__);
        exit(1);
    }

    printf("Need path to your history file: ");
    //if( fgets(input_path, sizeof(input_path), stdin) != NULL )
    if( EOF != scanf("%s", input_path))
    {
        // Save path to history file
        fprintf(fp, "hist_path=%s\n", input_path);

        // Save initial number of commits
        num_commits = check_commits(input_path);
        fprintf(fp, "commits_total=%d\n", num_commits);
    }

    fclose(fp);
}

char *get_path_to_hist()
{
    int i;
    char line[1024];
    char *hist_path = NULL;
    FILE *fp = NULL;

    if((fp = fopen(TERMAGACHI_LOCAL_FILE, "r")) == NULL )
    {
        printf("ERROR: %d: fp is null\n", __LINE__);
        exit(1);
    }

    while( fgets(line, sizeof(line), fp) != NULL )
    {
        if(strstr(line, "hist_path") != NULL )
        {
            asprintf(&hist_path, "%s", &line[10]);
            for(i = 0; hist_path != NULL; i++)
            {
                if(hist_path[i] == '\n')
                {
                    hist_path[i] = '\0';
                    break;
                }
            }
        }
    }

    return hist_path;
}

int check_commits(const char *hist_path)
{
    int count;
    char buf[4096];

    FILE *fp = NULL;

    if((fp = fopen(hist_path, "r")) == NULL )
    {
        printf("ERROR: %d: fp is null\n", __LINE__);
        exit(1);
    }

    while( fgets(buf, sizeof(buf), fp) != NULL )
    {
        if(strstr(buf, "commit") != NULL )
            count++;
    }

    fclose(fp);

    return count;
}

_MOOD play_game()
{
    char input[4];
    int correct_num, guess;
    int guessed_it = 0, num_tries = 0;
    
    _MOOD mood = LOSE;
    correct_num = (rand() % 100) + 1;

    printf("\nLet's play \"Guess my number.\" You have 10 tries to guess the number I'm thinking of. Give me a guess and press enter!\n");

    for(; num_tries < 10; num_tries++)
    {
        printf("Guess #%d: ", num_tries + 1);
        if( fgets(input, sizeof(input), stdin) != NULL )
        {
            guess = atoi(input);
            if(guess < correct_num)
            {
                printf("Nope! Higher!\n");
            }
            else if(guess > correct_num)
            {
                printf("Wrong! Loweeerrrr...\n");
            }
            else
            {
                printf("Lucky guess, punk.\n");
                guessed_it = 1;
                break;
            }
        }
    }

    if(num_tries == 10 && guessed_it == 0)
        mood = WIN;
    else
        mood = LOSE;

    return mood;
}

int main(int argc, char **argv)
{
    char *hist_path = NULL;
    FILE *fp = NULL;

    // If not already set up, get set up.
    if( (fp = fopen(TERMAGACHI_LOCAL_FILE, "r")) == NULL )
        setup();

    // Print the help menu
    if(argc < 2)
    {
        print_help();
        exit(1);
    }

    _MOOD mood = REGULAR;

    if(strcmp("status", argv[1]) == 0)
    {
        hist_path = get_path_to_hist();
        
        if(check_commits(hist_path) < 40)
            mood = HUNGRY;
        else
            mood = HAPPY;
        
        free(hist_path);
    }
    else if(strcmp("sayhi", argv[1]) == 0)
    {
        mood = REGULAR;
    }
    else if(strcmp("play", argv[1]) == 0)
    {
        mood = PLAY;
        print_termagachi(mood);

        mood = play_game();
    }
    else
    {
        print_termagachi(mood);
    }

    return 0;
}
