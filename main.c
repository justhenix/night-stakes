/* RESPONSI I - 08.Tema Hiburan/Game - Night Stakes
 * Nama  : Gamma Assyafi Fadhillah Ar Rasyad
 * NIM.  : L0125013
 * Kelas : A
 * Prodi : Informatika
 */

// It's recommended to run the .exe or built version to get a better experience,
// or using Embarcadero Dev-C++ to test from IDE (because automatically launch console),
// also maximize or fullscreen the CLI.
// I hope this program doesn't confusing :)

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

// Clear screen stuff
#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
	#define CLEAR_CMD "cls"
	#define GETCH() getch()
#else
	#include <unistd.h>
	#define CLEAR_CMD "clear"
	static int getch_fallback(void) { return getchar(); }
	#define GETCH() getch_fallback()
#endif

// Text style
#define TEXT_RESET "\033[0m"
#define TEXT_BOLD  "\033[1m"
#define TEXT_RED   "\033[91m"
#define TEXT_GREEN "\033[92m"
#define TEXT_YELLOW "\033[93m"

// Core game function
void welcome(void);
void main_menu(void);
void display_delay(int ms);
void clear_screen(void);
void start(void);
void credit(void);
void bye_msg(void);
void typefx(const char *text, int delay_ms);
void reset_player(void);
void play_sequence(void);
bool note_fullscreen(void);
bool check_if_broke(void);

// Story
void story_intro(void);
int story_before_rr(void);
void ending_scene(void);

// Minigames function
int tic_tac_toe(void);
int math_quiz(void);
int russian_roulette(void);

// Player data
typedef struct player {
	char name[50];
	int money;
	int live;     // how many lives or health point
	bool alive;
	bool walkedAway;
} Player;

Player gPlayer;   // global player

// Misc
int c;
int delay_ms = 30;

int main(void) {
    srand((unsigned)time(NULL));
    	
    note_fullscreen();
    welcome();
    main_menu(); // From here, the game will be going.

    return 0;
}

void clear_screen(void) {
    system(CLEAR_CMD);
}

// Let's anticipate about crazy user XD
// if the user spamming keyboard like a crazy
// we need to remove spam that happened before prompt
static void flush_input_buffer(void) {
#ifdef _WIN32
    // Windows console version
	// - Microsoft documentation regarding this stuff
	// check out https://learn.microsoft.com/en-us/windows/console/getstdhandle
	// also this https://learn.microsoft.com/en-us/windows/console/flushconsoleinputbuffer
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hIn);
#else
    // Fallback for Linux or macOS terminal
    // Drain anything waiting on stdin until newline or EOF
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // throw away characters
    }
#endif
}

// press any key to continue
static void wait_for_keypress(void) {
#ifdef _WIN32
	flush_input_buffer();
	printf("\n\nPress any key to continue...");
	fflush(stdout);
	GETCH();
#else
	printf("\n\nPress Enter to continue...");
	fflush(stdout);
	while (getchar() != '\n') { }
#endif
	printf("\n");
}

// User needs to maximize or fullscreen
bool note_fullscreen(void) {
    for (;;) {
        char choice;
        puts("+================================================+");
        puts("|    Make sure to MAXIMIZE or FULLSCREEN (F11)   |");
        puts("|    this window to get a better experience      |");
        puts("+================================================+");
        printf("Have you done it? (y/n) ");
        fflush(stdout);
		
        if (scanf(" %c", &choice) != 1) {
            // clear invalid input
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            continue;
        }

        choice = (char)tolower((unsigned char)choice);

        if (choice == 'y') {
            puts("\nGreat! Let's continue...\n");
            display_delay(2000);
            clear_screen();
            return true;
        } else if (choice == 'n') {
            puts("\nPlease go fullscreen or maximize first");
            display_delay(2000);
            clear_screen();
            // loop again
        } else {
            puts("\nInvalid choice, please type y or n");
            display_delay(2000);
            clear_screen();
        }
    }
}

// Text delay
void display_delay(int ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000); // microseconds
#endif
}

// this function should be for removing leading and trailing whitespace and newline
static void str_trim(char *s) {
	size_t start = 0;
	while (s[start] && isspace((unsigned char)s[start])) {
		start++;
	}

	size_t end = strlen(s);
	while (end > start && isspace((unsigned char)s[end - 1])) {
		end--;
	}

	if (start > 0) {
		memmove(s, s + start, end - start);
	}

	s[end - start] = '\0';
}

// make string uppercase in place
static void str_uppercase(char *s) {
	for (int i = 0; s[i]; i++) {
		s[i] = (char)toupper((unsigned char)s[i]);
	}
}

// read a line from stdin, trim it, and if it is empty... ask again
static void read_line_nonempty(char *buf, size_t sz) {
	do {
		if (!fgets(buf, sz, stdin)) {
			buf[0] = '\0';
			return;
		}
		str_trim(buf);
	} while (buf[0] == '\0');
}

// Typewriter effect
void typefx(const char *text, int delay_ms) {
    for (int i = 0; i < strlen(text); i++) {
        putchar(text[i]);    // Print the character
        fflush(stdout);

        // Cross-platform delay
        #ifdef _WIN32
            Sleep(delay_ms); // Windows' delay (ms)
        #else
            // non-Windows delay using nanosleep
            struct timespec ts; // ts = timespec
            ts.tv_sec = delay_ms / 1000;
            ts.tv_nsec = (delay_ms % 1000) * 1000000;
            nanosleep(&ts, NULL);
        #endif
    }
}

// Welcome screen
void welcome(void) {
    printf(TEXT_RED);
    puts("\t\t +==================================================================================================================+");
    puts("\t\t       /$$   /$$ /$$           /$$         /$$            /$$$$$$   /$$               /$$");
    puts("\t\t      | $$$ | $$|__/          | $$        | $$           /$$__  $$ | $$              | $$");
    puts("\t\t      | $$$$| $$ /$$  /$$$$$$ | $$$$$$$  /$$$$$$        | $$  \\__//$$$$$$    /$$$$$$ | $$   /$$  /$$$$$$   /$$$$$$$");
    puts("\t\t      | $$ $$ $$| $$ /$$__  $$| $$__  $$|_  $$_/        |  $$$$$$|_  $$_/   |____  $$| $$  /$$/ /$$__  $$ /$$_____/");
    puts("\t\t      | $$  $$$$| $$| $$  \\ $$| $$  \\ $$  | $$           \\____  $$ | $$      /$$$$$$$| $$$$$$/ | $$$$$$$$|  $$$$$$ ");
    puts("\t\t      | $$\\  $$$| $$| $$  | $$| $$  | $$  | $$ /$$       /$$  \\ $$ | $$ /$$ /$$__  $$| $$_  $$ | $$_____/ \\____  $$");
    puts("\t\t      | $$ \\  $$| $$|  $$$$$$$| $$  | $$  |  $$$$/      |  $$$$$$/ |  $$$$/|  $$$$$$$| $$ \\  $$|  $$$$$$$ /$$$$$$$/");
    puts("\t\t      |__/  \\__/|__/ \\____  $$|__/  |__/  \\___/         \\______/   \\___/   \\_______/|__/  \\__/ \\_______/|_______/");
    puts("\t\t                     /$$  \\ $$ ");
    puts("\t\t                    |  $$$$$$/ ");
    puts("\t\t                     \\______/  ");
    puts("\t\t +=================================================================================================================+");
    printf(TEXT_RESET);
    fflush(stdout);

	typefx("\n                                                                  by Gamma Rasyad", 10);
    display_delay(2000);
   	typefx("\n\n\n\n                                                             Press any key to continue...", 10);
    GETCH();
    clear_screen();
}

void main_menu(void) {
    int menu_choice = 0;
    do {
        // Draw banner again
        clear_screen();
        printf(TEXT_RED);
        puts("\t\t +====================================================================================================================+");
        puts("\t\t       /$$   /$$ /$$           /$$         /$$            /$$$$$$   /$$               /$$");
        puts("\t\t      | $$$ | $$|__/          | $$        | $$           /$$__  $$ | $$              | $$");
        puts("\t\t      | $$$$| $$ /$$  /$$$$$$ | $$$$$$$  /$$$$$$        | $$  \\__//$$$$$$    /$$$$$$ | $$   /$$  /$$$$$$   /$$$$$$$");
        puts("\t\t      | $$ $$ $$| $$ /$$__  $$| $$__  $$|_  $$_/        |  $$$$$$|_  $$_/   |____  $$| $$  /$$/ /$$__  $$ /$$_____/");
        puts("\t\t      | $$  $$$$| $$| $$  \\ $$| $$  \\ $$  | $$           \\____  $$ | $$      /$$$$$$$| $$$$$$/ | $$$$$$$$|  $$$$$$ ");
        puts("\t\t      | $$\\  $$$| $$| $$  | $$| $$  | $$  | $$ /$$       /$$  \\ $$ | $$ /$$ /$$__  $$| $$_  $$ | $$_____/ \\____  $$");
        puts("\t\t      | $$ \\  $$| $$|  $$$$$$$| $$  | $$  |  $$$$/      |  $$$$$$/ |  $$$$/|  $$$$$$$| $$ \\  $$|  $$$$$$$ /$$$$$$$/");
        puts("\t\t      |__/  \\__/|__/ \\____  $$|__/  |__/  \\___/         \\______/   \\___/   \\_______/|__/  \\__/ \\_______/|_______/");
        puts("\t\t                     /$$  \\ $$ ");
        puts("\t\t                    |  $$$$$$/ ");
        puts("\t\t                     \\______/  ");
        puts("\t\t +===================================================================================================================+");
		printf(TEXT_RESET);
		
		printf(TEXT_BOLD);
        puts("\n                                                                  Main Menu");
        printf(TEXT_RESET);
        
        puts("\n\n                                                                  1. Play");
        puts("                                                                  2. Credit");
        puts("                                                                  3. Exit");
        printf("\n\n                                                              Choose menu 1 2 or 3: ");
        fflush(stdout);
		
        if (scanf("%d", &menu_choice) != 1) {
            // clear invalid input
            while ((c = getchar()) != '\n' && c != EOF);
            puts("\n\n                                                              Invalid input");
            display_delay(2000);
            clear_screen();
            menu_choice = 0;
            continue;
        }

        switch (menu_choice) {
            case 1:
                start();
				play_sequence();
                break;
            case 2:
                credit();
                break;
            case 3:
                bye_msg();
                break;
            default:
                puts("\n\n                                                              Invalid choice, try again");
                display_delay(2000);
                clear_screen();
        }
    } while (menu_choice != 3);
}

void start(void){
    clear_screen();
    // Intended loading animation (UX kind of thingy)
    printf("\n\n\t\t                                                              Starting game.");
    display_delay(1000);
    printf(".");
    display_delay(1000);
    printf(".");
    display_delay(2000);
    clear_screen();
}

void credit(void){
	clear_screen();
	printf("            == CREDIT ==\n\n");
	
	typefx("Program Name : Night Stakes\n", 25);
	typefx("Creator      : Gamma Assyafi Fadhillah Ar Rasyad\n", delay_ms);
	typefx("ID Number    : L0125013\n", delay_ms);
	typefx("Class        : A\n", delay_ms);
	typefx("Major        : Informatics\n", delay_ms);
	typefx("Subject      : Programming Concept\n", delay_ms);
	typefx("Assistant    : Andradhi Bondan Pamungkas and Muhamad Nabil Fannani\n", delay_ms);
	
	display_delay(2000);
	wait_for_keypress();
    clear_screen();
}

static void waiver_ascii(void) {
	puts("\t\t +=============================================================================+");
	puts("\t\t |     ___ ___ _  _ ___ ___    _   _       ___ ___ _    ___   _   ___ ___      |");
	puts("\t\t |    / __| __| \\| | __| _ \\  /_\\ | |     | _ \\ __| |  | __| /_\\ / __| __|     |");
	puts("\t\t |   | (_ | _|| .` | _||   / / _ \\| |__   |   / _|| |__| _| / _ \\\\__ \\ _|      |");
	puts("\t\t |    \\___|___|_|\\_|___|_|_\\/_/_\\_\\____|__|_|_\\___|____|___/_/_\\_\\___/___|     |");
	puts("\t\t |          / _ \\| __| | |  |_ _| /_\\ | _ )_ _| |  |_ _|_   _\\ \\ / /           |");
	puts("\t\t |         | (_) | _|  | |__ | | / _ \\| _ \\| || |__ | |  | |  \\ V /            |");
	puts("\t\t |          \\___/|_|   |____|___/_/ \\_\\___/___|____|___| |_|   |_|             |");
	puts("\t\t |                                                                             |");
	puts("\t\t | ::::.:::-:: :::: :.:: -. .::--.---::-----::-..: :.:-::..:::.:-.::: .:::.... |");
	puts("\t\t | .       .     .           ..   .  . .        .             .. .             |");
	puts("\t\t | ::::.:::-:: :::: :.:: -. .::--.---::-----::-..: :.:-::..:::.:-.::: .:::.... |");
	puts("\t\t | :::-::-.: :::: ::::- ::..- .:::-:::-:.::.:.. -:::--=.::-.:..: :.::-:.::     |");
	puts("\t\t | :.-.:....-._--.-.-.-:::--.-.-.-.-.-.....-::-..: :.:-::..:::.:-.::: .:::.... |");
	puts("\t\t | ::::.:::-:: :::: :.:: -. .::--.---::-----::-..: :.:-::..:::.:-.::: .:::.... |");
	puts("\t\t | :::-:.::.:.:. -:.:.---..-::::.--:.:.-----: :-:: :.::.--::::- .:.:.::::-.    |");
	puts("\t\t | ::::.:::-:: :::: :.:: -. .::--.---::-----::-..: :.:-::..:::.:-.::: .:::.... |");
	puts("\t\t | :::::..::.::::.:::-: -::: :.::.:.-:-.::::::.-.--.:::::::.:.::::.:::::::     |");
	puts("\t\t | ::::.:::-:: :::: :.:: -. .::--.---::-----::-..: :.:-::..:::.:-.::: .:::.... |");
}

static void ask_player_name(void) {
	for (;;) {
		printf("\t\t | Enter Name : ");
		fflush(stdout);

		if (!fgets(gPlayer.name, sizeof(gPlayer.name), stdin)) {
			// read failed just try again
			continue;
		}

		// remove newline and surrounding spaces
		str_trim(gPlayer.name);

		if (gPlayer.name[0] != '\0') {
			// got a real name
			break;
		}

		// still empty give in character warning and loop again
		typefx("\n\nDEALER: Write your proper name.\n", delay_ms);
		display_delay(600);
		clear_screen();
		waiver_ascii();
	}
}

void waiver(void){
	waiver_ascii();
	ask_player_name();

	puts("\t\t |                                                                             |");
	puts("\t\t +=============================================================================+");

	typefx("\n\nWelcome to the table, ", delay_ms);
	typefx(gPlayer.name, delay_ms);
	typefx(". Take a seat.\n", delay_ms);
}

void bye_msg(void){
	clear_screen();
    printf(TEXT_BOLD);
    puts("\t\t +======================================+");
    display_delay(500);
    puts("\t\t |                                      |");
    display_delay(500);
	printf(TEXT_BOLD);
	printf(TEXT_YELLOW);
    puts("\t\t |          THANKS FOR PLAYING!         |");
    display_delay(500);
	printf(TEXT_RESET);
    puts("\t\t |                                      |");
    display_delay(500);
    puts("\t\t +======================================+");
    display_delay(600);
    printf(TEXT_RESET);
}

// Reset player's data for new game
void reset_player(void){
	memset(&gPlayer, 0, sizeof(gPlayer));
	gPlayer.money = 1000; // starting cash player bring to the Basement
	gPlayer.live  = 1;
	gPlayer.alive = true;
	gPlayer.walkedAway = false; // Initialize new field
}

bool check_if_broke(void) {
	if (gPlayer.money <= 0) {
		gPlayer.money = 0; // Avoid negative values by set money to 0
		clear_screen();
		typefx("DEALER: You're out of chips.\n", delay_ms);
		display_delay(600);
		typefx("DEALER: The house always wins.\n", delay_ms);
		wait_for_keypress();
		clear_screen();
		return true; // Yes, the player is broke
	}
	return false; // Player is not broke
}

// Mini games sequence
void play_sequence(void) {
	reset_player();   // money = 1000 etc

	story_intro();
	waiver();
	wait_for_keypress();
	clear_screen();

	tic_tac_toe();    // Minigame 1
	if (check_if_broke()) {
		goto skip_to_end; // Skip if broke
	}
	wait_for_keypress();
	clear_screen();

	math_quiz();      // Minigame 2
	if (check_if_broke()) {
		goto skip_to_end; // Skip if broke
	}
	wait_for_keypress();
	clear_screen();

	// Pre-RR choice: walk or risk it
	int will_play_rr = story_before_rr();
	wait_for_keypress();
	clear_screen();

	if (will_play_rr == 1) {
		// Player agreed to play Russian roulette
		russian_roulette(); // Minigame 3
		wait_for_keypress();
		clear_screen();
	} else {
		// Player walked away from last minigame
		// Mark alive just to be safe
		gPlayer.alive = true;
	}

	skip_to_end: // This is for our goto jump
		// Final ending based on money, alive, walkedAway
		ending_scene();

	wait_for_keypress();
	clear_screen();
}

void story_intro(){
	// Where am I?
	typefx("You are in a room.", delay_ms);
	display_delay(600);
	typefx("\nIt is dark. A single, weak bulb hangs over a green felt table, casting long shadows.", delay_ms);
	display_delay(600);
	typefx("\nThe air is thick with stale smoke and the smell of cheap whiskey.", delay_ms);
	display_delay(600);
	typefx("\nYou feel the cold weight of debt in your gut. This is the brink.", delay_ms);
	display_delay(600);

	// --- Litte background story ---
	typefx("\n\nYou remember the “sure win” bet that turned out not to be.", delay_ms);
	display_delay(500);
	typefx("\nYou remember the slick paper of the final eviction notice.", delay_ms);
	display_delay(500);
	typefx("\nYou remember the doctor's voice on the phone... 'The insurance won't cover this part...'", delay_ms);
	display_delay(500);
	typefx("\nIt wasn't one big mistake. It was a thousand small cuts that bled you dry.", delay_ms);
	display_delay(600);
	typefx("\nAnd it all led you here.", delay_ms);
	display_delay(600);

	wait_for_keypress();
	clear_screen();

	typefx("Across the table, someone shuffles a deck of cards and noticed you.\n\n", delay_ms);
	display_delay(600);

	puts("\t\t ----=+=-==+++**+=--===++++--------=++=--------==++**+=------------=+*=--+*=---------------------==++");
	puts("\t\t -=+=+++=--=++++=---------=***++==--=----+*##+=---+#*=----------=*#*+==-=+-=%=-=+=++**++=--==--------");
	puts("\t\t -=*#*#**+**==---------+*==-------==-=##+=--+*+=-----=*#=----=++==-----=*==*++-----===**+==**+=------");
	puts("\t\t *-=*-----=####**+==#+--------------%+---=+---------=---*#------------=*-++=#--+##+=------------+#*=-");
	puts("\t\t *+=+------*--------=+%------==+=-=#=--------*@@@@%*==+=--@*---------++-#+*==+=------=**+++=---------");
	puts("\t\t *+=+------*----=-----#--++==++=--**=+-----+@@@@@@@@@@*---+@+-------=*-+*#+==----------=++===+**+----");
	puts("\t\t =++=------*#*+*=-----#==**=+*=---%*+-=*-=%@@@@@@@@@@@@@#--#%-----===*--@+---------=+*=+=--=--=+*=-++");
	puts("\t\t -=+=-----------------%++*+==----+@+-++-*@@@@#+-----=#@@@@+-##==-----=*=-*+---==++++===+++=+=-------=");
	puts("\t\t --+=-----------------#-==*+=*---@+-#==%@@%*++===*--=-+#@@@*-##--------=#==#-------------=+****++====");
	puts("\t\t --+=----------------=#+=+*=++=-%*-+-=@@*===----+++----=-*%@*-#+------++-*==%---=*+=-----*-+=--==----");
	puts("\t\t --+=----------------=#+++=-===**---+@@=#@@@@@#-+-#-+@@@@+@@@-#*=+++===--#=-%---=*-------*-*---+=----");
	puts("\t\t --+=--+=------------=*-++=+*==#=--*@@@+#@@@@@@+=-++@@@@@+@@@+#*--=++=**=-=#=-+*++-------*-----*=----");
	puts("\t\t #%#*==*--=*#*+==-----*+*=----=%=-+@@@=--%@@@%#-=@%-*@@@@=-@@##*----=*-+%*=-----=*-------+*=**-------");
	puts("\t\t -==+=+*------+--=+#++#++=----+@*-*@@@*++--==---#@@+-=+==+=%@@*#----+++=----====++-------------------");
	puts("\t\t -==+=+*------+----*-----*=---%#@=+@@@@@@%@#%+--+=++++%@%@@@@@+%=----#++-==**+===+-------------------");
	puts("\t\t ------*------+----*-----*=--=%=%@=%@@@@+*@@*+=--===+#@@%@@@@@*@+--=***+-=+**+==++-------------------");
	puts("\t\t ------*-------=++++-----*=-=+%#=@#+@@@@+=%@@##+#+%*%%*#%@@@@*%#=#**++---=+=----=+--===--------------");
	puts("\t\t ------*-----------------*@@#==%=-#**@@@%-+#%%%@@@@%%@@+=*@@*@@+*=*------=+-----=+-------------------");
	puts("\t\t ------*-------------+#%+-----=#%-=%=#@@@@#==+###**#*#%@@##+*#%##*=---====+-----=***+=--------=+**+==");
	puts("\t\t ------*----------=#*=---------=@@+-%+#@@@@@#===++--+#@@@@*#+#%+*%#=-----=+=====++==++++*********==--");
	puts("\t\t ------*---------=@==*#=-=#------@@%-#+%@@@@@@+-----*@@@@=%@##+---=#@+--++--------------------*==-===");
	puts("\t\t ------*--------=#=----=*==*-----*@%@%#=*@@@@@@@@@@@@@@%*@@%=---*----**-++----=+--------------*+*=---");
	puts("\t\t ***+**#***+++=-**------=#==+-=+-=@++%@%==%@@@@@@@@@@@#*%@%=---=+--+-=%=++---=+*--------------*=+-+--");
	puts("\t\t -==-----==-----#+----=*--%*#=-+=-%%--+@@*-+@@@@@@@@@#@*%#-----=+-++-=%+++--------------------*==-+--");
	puts("\t\t -+=-----++=-=-*#------=*-=%%*-+=-+@+-=%@@%+=%@@@@@@@@*#*-++---+==#---##=+--------------------+==----");
	puts("\t\t -+=-----+*=-=%+--------=#-*@%-+=-=%@--*@*+@@%%@@@@@@***-=#----++@=+--*@*-----------=**###---+#==++++");
	puts("\t\t -+=---------#+------=---=%=%@-*=*-+@*--@@+-=*@@@@@@+*+-=%=---=*%+++--=%@*----------*+@@#%*#%*-++==**");
	puts("\t\t +==--------=#=-------%#=-*##@-*=%*-=%=--%%------------=%*----+@*-#=---#=#*---------#%+=#%@#%----===-");
	puts("\t\t +==--------#*---------+@#=%%@=#=@@=------=----#%%#**#*=*-----%#-**-+=-+*-*%*-------=**%%#*++++#*---=");
	fflush(stdout);

	display_delay(400);
	wait_for_keypress();
	clear_screen();

	typefx("\nDEALER: \"You know the rules.\"", delay_ms);
	display_delay(400);
	typefx("\nDEALER: \"One night. Win it all, or lose it all.\"", delay_ms);
	display_delay(400);
	typefx("\nYOU: This is all I have.", delay_ms);
	display_delay(400);

	// Display stats
	printf(TEXT_BOLD TEXT_YELLOW "\n\n\t\t[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
	display_delay(200);

	typefx("\nDEALER: Fine.", delay_ms);
	display_delay(400);

	typefx("\nDEALER: First, sign the waiver please.", delay_ms);
	display_delay(400);
	typefx("\nDEALER: Just... in case.\n\n", delay_ms);

	display_delay(1000);
}

// ===== eX Ou helpers =====
static void ttt_draw_board(const char b[9]) {
    printf("\n");
    printf("             %c | %c | %c\n", b[0], b[1], b[2]);
    printf("            ---+---+---\n");
    printf("             %c | %c | %c\n", b[3], b[4], b[5]);
    printf("            ---+---+---\n");
    printf("             %c | %c | %c\n\n", b[6], b[7], b[8]);
}

static int ttt_lines[8][3] = {
    {0,1,2},{3,4,5},{6,7,8}, // rows
    {0,3,6},{1,4,7},{2,5,8}, // cols
    {0,4,8},{2,4,6}          // diagonals
};

// return  1 if player X wins
// return -1 if dealer O wins
// return  2 if tie
// return  0 if ongoing
static int ttt_check(const char b[9]) {
    for (int i = 0; i < 8; i++) {
        int a = ttt_lines[i][0], c = ttt_lines[i][1], d = ttt_lines[i][2];
        if (b[a] == b[c] && b[c] == b[d]) {
            if (b[a] == 'X') return 1;
            if (b[a] == 'O') return -1;
        }
    }
    for (int i = 0; i < 9; i++) if (b[i] >= '1' && b[i] <= '9') return 0;
    return 2;
}

static int ttt_pick_random(const char b[9]) {
    int freeIdx[9], n = 0;
    for (int i = 0; i < 9; i++) if (b[i] != 'X' && b[i] != 'O') freeIdx[n++] = i;
    if (n == 0) return -1;
    return freeIdx[rand() % n];
}

// simple smart AI
static int ttt_ai_smart(char b[9], char me, char opp) {
    // 1. win if possible
    for (int i = 0; i < 8; i++) {
        int a = ttt_lines[i][0], c = ttt_lines[i][1], d = ttt_lines[i][2];
        if (b[a] != 'X' && b[a] != 'O' && b[c] == me && b[d] == me) return a;
        if (b[c] != 'X' && b[c] != 'O' && b[a] == me && b[d] == me) return c;
        if (b[d] != 'X' && b[d] != 'O' && b[a] == me && b[c] == me) return d;
    }
    // 2. block opponent
    for (int i = 0; i < 8; i++) {
        int a = ttt_lines[i][0], c = ttt_lines[i][1], d = ttt_lines[i][2];
        if (b[a] != 'X' && b[a] != 'O' && b[c] == opp && b[d] == opp) return a;
        if (b[c] != 'X' && b[c] != 'O' && b[a] == opp && b[d] == opp) return c;
        if (b[d] != 'X' && b[d] != 'O' && b[a] == opp && b[c] == opp) return d;
    }

    // 3. center
    if (b[4] != 'X' && b[4] != 'O') return 4;

    // 4. corners
    int corners[4] = {0,2,6,8};
    for (int i = 0; i < 4; i++) if (b[corners[i]] != 'X' && b[corners[i]] != 'O') return corners[i];

    // 5. sides
    int sides[4] = {1,3,5,7};
    for (int i = 0; i < 4; i++) if (b[sides[i]] != 'X' && b[sides[i]] != 'O') return sides[i];
    return -1;
}

// Minigames
int tic_tac_toe(void) {
    const int penalty[3] = {200, 250, 300};
    int net = 0;

    clear_screen();
    typefx("DEALER: Warm up. Three rounds of eX Ou.\n", delay_ms);
    display_delay(600);
    typefx("Rules: You are X. Dealer is O. Win gives +350. Lose subtracts based on the round.\n\n", delay_ms);
    display_delay(2000);

    for (int round = 1; round <= 3; round++) {
        char b[9] = {'1','2','3','4','5','6','7','8','9'};
        int result = 0; // 0 ongoing 1 player  -1 dealer  2 tie
        int turn_is_player = 1; // player starts first

        clear_screen();
        printf(TEXT_BOLD "eX Ou Round %d\n" TEXT_RESET, round);
        printf(TEXT_YELLOW "[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
        ttt_draw_board(b);

        while ((result = ttt_check(b)) == 0) {
            if (turn_is_player) {
                // player turn
                int move = -1;
                for (;;) {
                    printf("Pick a cell 1-9 then Enter: ");
                    fflush(stdout);

                    char line[32];
                    if (!fgets(line, sizeof(line), stdin)) {
                        continue;
                    }

                    char *endp = NULL;
                    long v = strtol(line, &endp, 10);

                    if (v < 1 || v > 9) {
                        puts("Invalid cell");
                    	display_delay(300);
                        continue;
                    }

                    move = (int)(v - 1);

                    if (b[move] == 'X' || b[move] == 'O') {
                        puts("Taken cell");
                    	display_delay(300);
                        continue;
                    }

                    break;
                }

                b[move] = 'X';
            	result = ttt_check(b);
            	if (result != 0) break;

            } else {
                // dealer turn
                int move;
                if (round == 1) {
                    // round 1 dealer is dumb random
                    move = ttt_pick_random(b);
                } else {
                    // round 2 and 3 dealer gets smarter
                    move = ttt_ai_smart(b, 'O', 'X');
                }

                if (move < 0) {
                    break;
                }

                b[move] = 'O';
                typefx("Dealer plays.\n", 15);
            	display_delay(600);
            }

            clear_screen();

        	printf(TEXT_BOLD "eX Ou Round %d\n" TEXT_RESET, round);
        	printf(TEXT_YELLOW "[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
        	ttt_draw_board(b);

            turn_is_player = !turn_is_player;
        }

        // result settle
        if (result == 1) {
            typefx("You win the round.\n", delay_ms);
            gPlayer.money += 350;
            net += 350;
            
            printf(TEXT_GREEN "[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
        	wait_for_keypress();
        	clear_screen();
        } else if (result == -1) {
            typefx("Dealer wins the round.\n", delay_ms);
            gPlayer.money -= penalty[round - 1];
            net -= penalty[round - 1];
            
        	printf(TEXT_RED "[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
        	wait_for_keypress();
        	clear_screen();
        } else {
            typefx("It is a draw.\n", delay_ms);
            // draw -> no money change

        	printf(TEXT_RESET "[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
        	wait_for_keypress();
        	clear_screen();
        }

        clear_screen();
    }

	if (gPlayer.money >= 1800) {
		typefx("YOU: Child's play isn't it?\n", delay_ms);
		display_delay(400);
		typefx("DEALER: Don't be so confident, ", delay_ms);
		typefx(gPlayer.name, delay_ms);
		printf(".\n");
	} else if (gPlayer.money >= 1000 && gPlayer.money <= 2000) {
		typefx("DEALER: Not bad at all.\n", delay_ms);
		display_delay(400);
	} else {
		typefx("DEALER: Careful now.\n", delay_ms);
	}

    typefx("DEALER: Warm up done.\n", delay_ms);
	typefx("DEALER: Next up, let's play some numbers.\n", delay_ms);
    return net;
}

// Math quiz minigame
int math_quiz(void) {
    // money stakes
    // round 1 reward 500 or minus 500 if wrong
    // round 2 reward 700 or minus 700 if wrong
    const int reward[2]  = {500, 700};
    const int penalty[2] = {500, 700};

    int net = 0; // track how much total money changed in this minigame

    clear_screen();

    typefx("DEALER: Let's see if your brain can save you now.\n", delay_ms);
    display_delay(600);
    typefx("DEALER: Two rounds. Answer wrong and I take your money.\n\n", delay_ms);
    display_delay(2000);

    // We run exactly 2 rounds
    for (int round = 1; round <= 2; round++) {

        clear_screen();

        if (round == 1) {
            typefx("SMART TEST Round 1\n", delay_ms);
            typefx("Basic math. Basic stuff.\n\n", delay_ms);
        } else {
            typefx("SMART TEST Round 2\n", delay_ms);
            typefx("Mind games. Think before you talk.\n\n", delay_ms);
        }

        printf(TEXT_YELLOW "[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);

        int qIndex = rand() % 3; // randomize the question that would be appear
        char answer[128];
        bool correct = false;

        if (round == 1) {
            // Round 1 question pool
            switch (qIndex) {

                case 0: {
                    // arithmetic expression with + - * and so on
                    printf("What is the result of the following calculation?\n");
                    printf("1 + 0 + 1 - 1 * 1 + 1 * 0 + 1 * 1 = ?\n> ");

                    read_line_nonempty(answer, sizeof(answer));

                    char *endp;
                    long val = strtol(answer, &endp, 10);

                    if (val == 2) {
                        correct = true;
                    }

                    if (correct) {
                        typefx("\nCorrect.\n", delay_ms);
                    } else {
                        typefx("\nWrong.\n", delay_ms);
                        typefx("Multiplication goes first.\n", delay_ms);
                        typefx("It becomes 1 + 0 + 1 - 1 + 0 + 1 which equals 2.\n", delay_ms);
                    }
                } break;

                case 1: {
                    // calculator product 0..9
                    printf("If you multiply all the numbers on a standard number pad 0 through 9,\n");
                    printf("what is the result?\n> ");

                    read_line_nonempty(answer, sizeof(answer));

                    char *endp;
                    long val = strtol(answer, &endp, 10);

                    if (val == 0) {
                        correct = true;
                    }

                    if (correct) {
                        typefx("\nCorrect.\n", delay_ms);
                    } else {
                        typefx("\nWrong.\n", delay_ms);
                        typefx("Any product with 0 inside becomes 0.\n", delay_ms);
                    }
                } break;

                default: {
                    // rope cut problem
                    printf("You have a single rope that is 10 meters long.\n");
                    printf("You want to cut it into pieces that are each 2 meters long.\n");
                    printf("How many cuts do you need to make?\n> ");

                    read_line_nonempty(answer, sizeof(answer));

                    char *endp;
                    long val = strtol(answer, &endp, 10);

                    // 10 meter rope, want 2 meter pieces
                    // That means 5 pieces total
                    // You only cut at 2 4 6 8 meter mark, so 4 cuts
                    if (val == 4) {
                        correct = true;
                    }

                    if (correct) {
                        typefx("\nCorrect.\n", delay_ms);
                    } else {
                        typefx("\nWrong.\n", delay_ms);
                        typefx("You end with 5 pieces.\n", delay_ms);
                        typefx("To create 5 pieces you only need 4 cuts.\n", delay_ms);
                    }
                } break;
            }

        } else {
            // Round 2 question pool
            switch (qIndex) {

                case 0: {
                    // sequence O T T F F S S E ?
                    printf("What is the next letter in this sequence?\n");
                    printf("O, T, T, F, F, S, S, E, __ ?\n> ");

                    read_line_nonempty(answer, sizeof(answer));
                    str_uppercase(answer);

                    // N for Nine
                    if (strcmp(answer, "N") == 0) {
                        correct = true;
                    }

                    if (correct) {
                        typefx("\nCorrect.\n", delay_ms);
                        typefx("O T T F F S S E N.\n", delay_ms);
                        typefx("Those are the first letters of One Two Three Four Five Six Seven Eight Nine.\n", delay_ms);
                    } else {
                        typefx("\nWrong.\n", delay_ms);
                        typefx("Answer is N.\n", delay_ms);
                        typefx("Each letter is the first letter of the word form of the number.\n", delay_ms);
                    }
                } break;

                case 1: {
                    // word length mapping
                    printf("Using the pattern below, what number does 10 equal?\n");
                    printf("7 = FIVE\n");
                    printf("6 = THREE\n");
                    printf("10 = ?\n> ");

                    read_line_nonempty(answer, sizeof(answer));
                    str_uppercase(answer);

                    // TEN has 3 letters so answer is THREE
                    if (strncmp(answer, "THREE", 5) == 0) {
                        correct = true;
                    }

                    if (correct) {
                        typefx("\nCorrect.\n", delay_ms);
                        typefx("TEN has 3 letters so the answer is THREE.\n", delay_ms);
                    } else {
                        typefx("\nWrong.\n", delay_ms);
                        typefx("Answer is THREE because TEN has 3 letters.\n", delay_ms);
                    }
                } break;

                default: {
                    // bat and ball 1.10 total
                    printf("A bat and a ball cost 1.10 dollars total.\n");
                    printf("The bat costs 1.00 dollar more than the ball.\n");
                    printf("How much does the ball cost in dollars?\n> ");

                    read_line_nonempty(answer, sizeof(answer));
                    str_uppercase(answer);

                    // accept 0.05 style answers
                    if (strncmp(answer, "0.05", 4) == 0 || strncmp(answer, "0,05", 4) == 0) {
                        correct = true;
                    }

                    if (correct) {
                        typefx("\nCorrect.\n", delay_ms);
                        typefx("Ball costs 0.05 dollars. Bat costs 1.05 dollars.\n", delay_ms);
                        typefx("0.05 + 1.05 = 1.10 total.\n", delay_ms);
                    } else {
                        typefx("\nWrong.\n", delay_ms);
                        typefx("Ball is 0.05 dollars, bat is 1.05 dollars. That makes 1.10.\n", delay_ms);
                    }
                } break;
            }
        }

        // settle money for this round
        if (correct) {
            gPlayer.money += reward[round - 1];
            net += reward[round - 1];
            typefx("\nDealer slides some chips your way.\n", delay_ms);
        } else {
            gPlayer.money -= penalty[round - 1];
            net -= penalty[round - 1];
            typefx("\nDealer drags money off your side of the table.\n", delay_ms);
        }

        printf(TEXT_YELLOW "\n[ MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);
    	wait_for_keypress();
        clear_screen();
    }

    // after round 2
    typefx("DEALER: Brains are fine.\n", delay_ms);
    display_delay(400);
    typefx("DEALER: Now, let's see if luck likes you.\n", delay_ms);
    display_delay(800);

    return net;
}


int story_before_rr(void){
	char choice;

	printf("*slide*\n");
	display_delay(600);
	typefx("The dealer opens a dusty drawer.\n", delay_ms);
	display_delay(800);
	typefx("DEALER: Look at this ol' lady, with this you can win more than ever before.\n\n	", delay_ms);
	display_delay(400);

	printf("\t\t                                    /=>\n");
	puts("\t\t     +____________________/\\/\\___ / /|");
	puts("\t\t   .''._____________'._____      / /|/\\ ");
	puts("\t\t  : () :              :\\ ----\\|    \\ )");
	puts("\t\t   '..'______________.'0|----|      \\");
	puts("\t\t                         |----    /----\\");
	puts("\t\t                        || -\\\\ --|      \\");
	puts("\t\t                        ||   || ||\\      \\");
	puts("\t\t                         \\\\____// '|      \\");
	puts("\t\t                                 .'/       |");
	puts("\t\t                                .:/        |");
	puts("\t\t                                :/_________|");
	fflush(stdout);
	display_delay(2500);

	clear_screen();

	typefx("\nDEALER: But, you can get out from here while you can...\n	", delay_ms);
	display_delay(400);

	clear_screen();

	typefx("DEALER: Or maybe you wanna try?\n", delay_ms);
	display_delay(200);
	typefx("\nTry or walk out (y/n) : ", delay_ms);
	fflush(stdout);

	for (;;) {
		if (scanf(" %c", &choice) != 1) {
			// clear invalid input
			while ((c = getchar()) != '\n' && c != EOF);
			clear_screen();
			typefx("\nTry or walk out (y/n) : ", delay_ms);
			continue;
		}

		choice = (char)tolower((unsigned char)choice);

		if (choice == 'y') {
			typefx("\nYOU: I'll do it.\n", delay_ms);
			display_delay(2000);
			clear_screen();
			return 1; // will continue to russian_roulette
		} else if (choice == 'n') {
			typefx("\nYOU: No...", delay_ms);
			display_delay(600);
			typefx("Not this time.\n", delay_ms);
			display_delay(2000);
			clear_screen();

			// mark that player walked out instead of playing russian roulette
			gPlayer.walkedAway = true;
			return 0; // SKIP russian_roulette
		} else {
			puts("\nInvalid choice, please type y or n");
			display_delay(2000);
			clear_screen();
		}
	}
}

int russian_roulette(void){
	clear_screen();

	typefx("The room goes quiet.\n", delay_ms);
	display_delay(600);
	typefx("Dealer places a heavy revolver on the table.\n", delay_ms);
	display_delay(600);
	typefx("He cracks the cylinder open. One round goes in.\n", delay_ms);
	display_delay(600);
	typefx("He spins it.\n", delay_ms);
	display_delay(600);
	typefx("It rattles like loose bones.\n\n", delay_ms);
	display_delay(1000);

	puts("\t\t                                     _");
	puts("\t\t                                    /=>");
	puts("\t\t     +____________________/\\/\\___ / /|");
	puts("\t\t   .''._____________'._____      / /|/\\ ");
	puts("\t\t  : () :              :\\ ----\\|    \\ )");
	puts("\t\t   '..'______________.'0|----|      \\");
	puts("\t\t                         |----    /----\\");
	puts("\t\t                        || -\\\\ --|      \\");
	puts("\t\t  LOADED                 ||   || ||\\      \\");
	puts("\t\t                         \\\\____// '|      \\");
	puts("\t\t                                 .'/       |");
	puts("\t\t                                .:/        |");
	puts("\t\t                                :/_________|");
	fflush(stdout);
	display_delay(1500);

	clear_screen();

	typefx("\nDEALER: Last shot.\n", delay_ms);
	display_delay(600);
	typefx("DEALER: You walk out loaded if you live.\n", delay_ms);
	display_delay(600);
	typefx("DEALER: Or you do not walk out...\n\n", delay_ms);
	display_delay(1000);

	// suspense prompt
	flush_input_buffer();
	printf(TEXT_YELLOW "[ MONEY: %d ]\n" TEXT_RESET, gPlayer.money);
	typefx("\nPress any key to pull the trigger...", delay_ms);
	fflush(stdout);
	GETCH();

	clear_screen();
	// simulate cylinder
	// bullet in random chamber 0...5
	int bullet_chamber = rand() % 6;
	// chamber fired 0...5
	int fire_chamber   = rand() % 6;

	// reveal result with little drama
	typefx("*click*\n", delay_ms);
	display_delay(1200);

	if (fire_chamber == bullet_chamber) {
		// player died
		printf("*BANG*");
		display_delay(600);

		clear_screen();

		typefx("???: Finally, a deserved peace.\n", 60);
		display_delay(600);
		typefx("???: What a poor soul.\n", 60);
		display_delay(1200);

		gPlayer.alive = false;
		gPlayer.live  = 0;
		gPlayer.money = 0;

		// return 0 means dead
		return 0;

	} else {
		// player survives
		typefx("\nSilence.\n", delay_ms);
		display_delay(600);
		typefx("You are still here.\n", delay_ms);
		display_delay(600);

		typefx("\nDealer laughs under his breath.\n", delay_ms);
		display_delay(600);
		typefx("DEALER: You lucky little ghost...\n", delay_ms);
		display_delay(800);

		// jackpot money for surviving
		int jackpot = 100000;
		gPlayer.money += jackpot;

		printf(TEXT_GREEN "\n[ +%d MONEY ]\n" TEXT_RESET, jackpot);
		printf(TEXT_YELLOW "[ TOTAL MONEY: %d ]\n\n" TEXT_RESET, gPlayer.money);

		gPlayer.alive = true;
		gPlayer.live  = 1;

		// return 1 means survived
		return 1;
	}
}


void ending_scene(void){
	// Use a slightly slower delay for the ending text to be more dramatic
    int end_delay_ms = 25;

    clear_screen();
    display_delay(1500); // Pause for dramatic effect before the ending

    if (gPlayer.alive == false) {
        // ENDING 4: PEACE (Died during Russian Roulette)
        // So we just show the "end" screen.
        printf(TEXT_RED TEXT_BOLD);
        puts("\n\n\n\n");
        typefx("\t\t\t\t\tT H E   E N D", 75);
        puts("\n\n");
        typefx("\t\t\t\t\tENDING 4/4: PEACE", 50);
        printf(TEXT_RESET);
        puts("\n\n\n\n\n");
        display_delay(3000);

    } else {
        // Player is still alive and these decide the ending

        if (gPlayer.walkedAway == false) {
            // ENDING 1: HOLLOW (Survived Russian Roulette and won the jackpot)
            // This implies gPlayer.money is very high (>= 100000)

            typefx("The dealer shoves the stack of chips toward you.\n", end_delay_ms);
            display_delay(800);
            typefx("DEALER: \"A deal's a deal. Get out.\"\n\n", end_delay_ms);
            display_delay(1200);

            typefx("You stumble out of the basement, the chips heavy in your pocket.\n", end_delay_ms);
            display_delay(800);
            typefx("The sun is rising. The light hurts your eyes.\n", end_delay_ms);
            display_delay(800);
            typefx("You have the money. The debts are paid. The operation is covered.\n", end_delay_ms);
            display_delay(800);
            typefx("But the sound of the *click*... and the smell of gunpowder...\n", end_delay_ms);
            display_delay(800);
            typefx("That's never going to leave you.\n\n", end_delay_ms);
            display_delay(2000);

            printf(TEXT_YELLOW TEXT_BOLD); // Yellow for a hollow victory
            puts("\n\n\n\n");
            typefx("\t\t\t\t\tT H E   E N D", 75);
            puts("\n\n");
            typefx("\t\t\t\t\tENDING 1/4: HOLLOW", 50);
            printf(TEXT_RESET);
            puts("\n\n\n\n\n");
            display_delay(3000);

        } else {
            // Player chose to walk away before Russian Roulette

            if (gPlayer.money >= 3000) {
                // ENDING 2: ENOUGH (Walked away with 'enough' money)

                typefx("DEALER: \"Smart move.\"\n", end_delay_ms);
                display_delay(800);
                typefx("He nods, almost imperceptibly. A flicker... of respect?\n", end_delay_ms);
                display_delay(800);
                typefx("DEALER: \"Go on. Don't come back.\"\n\n", end_delay_ms);
                display_delay(1200);

                typefx("You walk out of the basement. The air is cold, still dark.\n", end_delay_ms);
                display_delay(800);
                typefx("It's not a jackpot. It's not enough to fix everything.\n", end_delay_ms);
                display_delay(800);
                typefx("YOU: It's enough. Enough for rent. Enough for a bus ticket.\n", end_delay_ms);
                display_delay(800);
                typefx("Enough for one more day.\n\n", end_delay_ms);
                display_delay(2000);

                printf(TEXT_GREEN TEXT_BOLD); // Green for the "good" ending
                puts("\n\n\n\n");
                typefx("\t\t\t\t\tT H E   E N D", 75);
                puts("\n\n");
                typefx("\t\t\t\t\tENDING 2/4: ENOUGH", 50);
                printf(TEXT_RESET);
                puts("\n\n\n\n\n");
                display_delay(3000);

            } else {
                // ENDING 3: NEXT TIME (Walked away with little/lost money)
                // (Money <= 2000, this 'else' covers < 3000)

                typefx("The dealer lets out a small laugh. It's not a kind sound.\n", end_delay_ms);
                display_delay(800);
                typefx("DEALER: \"Running? With... that?\"\n", end_delay_ms);
                display_delay(800);
                typefx("He gestures to your meager pile of chips.\n", end_delay_ms);
                display_delay(800);
                typefx("DEALER: \"You're just delaying the inevitable.\"\n\n", end_delay_ms);
                display_delay(1200);

                typefx("You push your chair back, the shame burning hotter than the whiskey.\n", end_delay_ms);
                display_delay(800);
                typefx("You're alive, but nothing is solved. The debts are still there.\n", end_delay_ms);
                display_delay(800);
                typefx("As the door closes, you hear him shuffling the deck again.\n", end_delay_ms);
                display_delay(800);
                typefx("He knows you'll be back. You know it, too.\n\n", end_delay_ms);
                display_delay(2000);

                printf(TEXT_BOLD); // Neutral/bold for the "bad" but alive ending
                puts("\n\n\n\n");
                typefx("\t\t\t\t\tT H E   E N D", 75);
                puts("\n\n");
                typefx("\t\t\t\t\tENDING 3/4: NEXT TIME", 50);
                printf(TEXT_RESET);
                puts("\n\n\n\n\n");
                display_delay(3000);
            }
        }
    }
}
