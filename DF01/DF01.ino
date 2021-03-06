// Nota: this app needs a modified version of the Gamebuino library
// with sound and keyboard stripped off, and enhanced gb.popup()
// If it was not provided to you when you downloaded this file,
// you can get it at http://mougino.free.fr/arduino

#define ENGINE_VER "v0.2 (24-AUG-2015)"
#define PCSOFT_VER "v0.1 (15-AUG-2015)"

#include <SPI.h>
#include <Gamebuino.h>
#include <petit_fatfs.h>
#include <EEPROM.h>

//#define debug // Comment for live engine. Uncomment for demo engine
#define FRENCH // Comment if you want menus in English
#define FONT5X7_C // Comment if you want only Font3x5, numKeyboard() won't be as pretty but it'll save memory

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// THIS IS THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK:
#define NPAR 400 // Number of paragraphs in this Gamebook
#ifdef FRENCH // Intro / init constants
#define LDV "DF01.LDV"
PROGMEM const char DF_title1[]="DEFIS FANTASTIQUES 01";
PROGMEM const char DF_title2[]="Le Sorcier de la Montagne de feu";
PROGMEM const char DF_eqp_ini[]="^epee\n"  "^armure de cuir\n"  "^sac a dos\n"  "^lanterne";
#else
#define LDV "FF01.LDV"
PROGMEM const char DF_title1[]="FIGHTING FANTASY 01";
PROGMEM const char DF_title2[]="The Warlock of Firetop Mountain";
PROGMEM const char DF_eqp_ini[]="^sword\n"  "^leather armour\n"  "^rucksack\n"  "^lantern";
#endif
// END OF THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// In-game labels
#ifdef FRENCH
// Adventure sheet constants
PROGMEM const char DF_adv[]="[FEUILLE D'AVENTURE]";
PROGMEM const char DF_ski[]="Habilete:";
PROGMEM const char DF_sta[]="Endurance:";
PROGMEM const char DF_luc[]="Chance:";
PROGMEM const char DF_gold1[]="Or:";
PROGMEM const char DF_gold2[]=" po";
PROGMEM const char DF_prov[]="Provisions:";
PROGMEM const char DF_meal[]="Endurance +4";
PROGMEM const char DF_pot255[]="Choisir ";
PROGMEM const char DF_potion[]="Potion ";
PROGMEM const char DF_pot0[]="d'Adresse";
PROGMEM const char DF_pot1[]="de Vigueur";
PROGMEM const char DF_pot2[]="Bon.Fortune";
PROGMEM const char DF_po_ski[]="Habilete niveau ini!";
PROGMEM const char DF_po_sta[]="Endurance niv. ini!";
PROGMEM const char DF_po_luc[]="Chance niveau ini+1";
PROGMEM const char DF_axn[]="[ACTIONS]";
PROGMEM const char DF_save[]="Sauver la partie";
PROGMEM const char DF_savok[]="Sauvegarde effectuee";
PROGMEM const char DF_roll1[]="Lancer un de";
PROGMEM const char DF_roll2[]="Lancer deux des";
PROGMEM const char DF_goto[]="Aller au...";
PROGMEM const char DF_about[]="A propos";
PROGMEM const char DF_eqp[]="[EQUIPEMENT]";
PROGMEM const char DF_add[]="Ajouter equipement";
// Game constants
PROGMEM const char DF_tyl[]="Tentez votre Chance";
PROGMEM const char DF_lucky[]="CHANCEUX";
PROGMEM const char DF_unlucky[]="MALCHANCEUX";
PROGMEM const char DF_fight[]="Combattez!";
PROGMEM const char DF_flee[]="Fuir le combat";
PROGMEM const char DF_you[]="VOUS";
PROGMEM const char DF_fski[]="HAB";
PROGMEM const char DF_fsta[]="END:";
// Menus
PROGMEM const char DF_m0s0[] = "Annuler";
PROGMEM const char DF_m1s1[] = "Modif. stat courante";
PROGMEM const char DF_m1s2[] = "Modif. total initial";
PROGMEM const char DF_m2s1[] = "Boire une mesure";
PROGMEM const char DF_m3s1[] = "Prendre un repas";
PROGMEM const char DF_m3s2[] = "Augmenter provisions";
PROGMEM const char DF_m4s1[] = "Supprimer equipement";
PROGMEM const char DF_m5s1[] = "Potion d'Adresse";
PROGMEM const char DF_m5s2[] = "Potion de Vigueur";
PROGMEM const char DF_m5s3[] = "P. de Bonne Fortune";
//-----------------------------------------------------------------------------
#else
// Adventure sheet constants
PROGMEM const char DF_adv[]="[ADVENTURE SHEET]";
PROGMEM const char DF_ski[]="Skill:";
PROGMEM const char DF_sta[]="Stamina:";
PROGMEM const char DF_luc[]="Luck:";
PROGMEM const char DF_gold1[]="Gold:";
PROGMEM const char DF_gold2[]=" gp";
PROGMEM const char DF_prov[]="Provisions:";
PROGMEM const char DF_meal[]=" Stamina +4";
PROGMEM const char DF_pot255[]="Choose ";
PROGMEM const char DF_potion[]="Potion ";
PROGMEM const char DF_pot0[]="of Skill";
PROGMEM const char DF_pot1[]="of Strength";
PROGMEM const char DF_pot2[]="of Fortune";
PROGMEM const char DF_po_ski[]="Skill @initial lvl!";
PROGMEM const char DF_po_sta[]="Stamina @init. lvl!";
PROGMEM const char DF_po_luc[]="Luck @initial lvl+1";
PROGMEM const char DF_axn[]="[ACTIONS]";
PROGMEM const char DF_save[]="Save game";
PROGMEM const char DF_savok[]="Game saved";
PROGMEM const char DF_roll1[]="Roll one die";
PROGMEM const char DF_roll2[]="Roll both dice";
PROGMEM const char DF_goto[]="Turn to...";
PROGMEM const char DF_about[]="About";
PROGMEM const char DF_eqp[]="[EQUIPMENT]";
PROGMEM const char DF_add[]="Add an equipment";
// Game constants
PROGMEM const char DF_tyl[]="Test your Luck";
PROGMEM const char DF_lucky[]="LUCKY";
PROGMEM const char DF_unlucky[]="UNLUCKY";
PROGMEM const char DF_fight[]="Fight!";
PROGMEM const char DF_flee[]="Escape battle";
PROGMEM const char DF_you[]="YOU";
PROGMEM const char DF_fski[]="ski";
PROGMEM const char DF_fsta[]="STA:";
// Menus
PROGMEM const char DF_m0s0[] = "Cancel";
PROGMEM const char DF_m1s1[] = "Change current stat";
PROGMEM const char DF_m1s2[] = "Change initial level";
PROGMEM const char DF_m2s1[] = "Drink one measure";
PROGMEM const char DF_m3s1[] = "Eat one meal";
PROGMEM const char DF_m3s2[] = "Add provision(s)";
PROGMEM const char DF_m4s1[] = "Remove equipment";
PROGMEM const char DF_m5s1[] = "Potion of Skill";
PROGMEM const char DF_m5s2[] = "Potion of Strength";
PROGMEM const char DF_m5s3[] = "Potion of Fortune";
#endif

Gamebuino gb;

#define ITMSIZE 17  // item of equipment: 17 characters max including starting '^' and final '\n' or '\0'
#define EQPSIZE 255 // equipment: 14 items of 17 chr (more if items < 17 characters)
#define NCHRX 21    // number of characters per line with font3x5
#define NCHRY 8     // number of lines per screen with font3x5
#define TXTSIZE 180 // text on screen: max 21x8 characters (with font3x5) + final '\0' + a few characters on next line (for active links)

#define EEPROM_IDENTIFIER 0xFFDF
#define PGMT(pgm_ptr) (reinterpret_cast <const __FlashStringHelper*> (pgm_ptr))
#define C_IS_SPECIAL_CHARACTER (c=='>' || c=='<' || c=='^')
#define TEST_YOUR_LUCK 0
#define ROLL_ONE_DIE 1
#define ROLL_BOTH_DICE 2
#define LEFT false
#define RIGHT true
#define CURRENT 100
#define PREVLINE 150
#define NEXTLINE 200

PROGMEM const char* DF_menu1[3] = {DF_m1s1, DF_m1s2, DF_m0s0}; // Edit stat menu
PROGMEM const char* DF_menu2[2] = {DF_m2s1, DF_m0s0}; // Drink potion menu
PROGMEM const char* DF_menu3[3] = {DF_m3s1, DF_m3s2, DF_m0s0}; // Provision menu
PROGMEM const char* DF_menu4[2] = {DF_m4s1, DF_m0s0}; // Delete equipment menu
PROGMEM const char* DF_menu5[4] = {DF_m5s1, DF_m5s2, DF_m5s3, DF_m0s0}; // Choose potion menu

PROGMEM const unsigned char DF_logo[] = 
{
  64,30,
  0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x6A,0x00,
  0x00,0x00,0x07,0xFE,0x00,0x00,0x39,0x00,
  0x00,0x00,0x10,0x40,0xF8,0x01,0xFA,0x00,
  0x00,0x00,0xB0,0xF9,0x8E,0x01,0xF8,0x00,
  0x00,0x0D,0x8F,0xFF,0x63,0xC7,0xF0,0x00,
  0x00,0x2B,0xFF,0xFF,0xFB,0x1F,0x00,0x00,
  0x00,0xC1,0xEF,0x7F,0x13,0xFC,0xE0,0x00,
  0x01,0x83,0xFD,0x3F,0x7B,0xFB,0xF0,0x00,
  0x03,0xEF,0xB8,0x91,0xF3,0xDF,0xE6,0x00,
  0x09,0xDF,0xFE,0xC6,0x14,0x6F,0xE1,0x80,
  0x10,0x3F,0xFF,0xCB,0x1F,0xF1,0xF3,0xA0,
  0x20,0x7F,0xFF,0xCE,0x01,0xFA,0xFB,0x30,
  0x58,0xFF,0xFF,0x44,0x47,0x41,0xBE,0x08,
  0xF8,0xFF,0xFC,0x50,0x1F,0xCF,0xBF,0x84,
  0xC0,0x07,0xEB,0x04,0xFF,0xF9,0x97,0xDC,
  0xC0,0x19,0xFA,0x81,0xFF,0xFE,0x2B,0xBA,
  0xFC,0x1A,0x0C,0x07,0xFF,0xFF,0x17,0xE0,
  0x1D,0xC0,0x38,0x5F,0xFF,0xFF,0xF7,0xF1,
  0x07,0xB0,0x30,0x3F,0xFF,0xFF,0xF7,0xF5,
  0x00,0x3F,0xC0,0xF8,0x1F,0xFF,0xFE,0x07,
  0x00,0x01,0x03,0xFC,0x9C,0x00,0x07,0x01,
  0x00,0x04,0x0F,0xE0,0x18,0x06,0x07,0x0E,
  0x00,0x00,0x17,0xF8,0x00,0x87,0x02,0x1E,
  0x00,0x20,0x40,0x1F,0xFC,0x00,0x0F,0xF0,
  0x00,0x83,0x00,0x00,0x3F,0xFF,0xFC,0x00,
  0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
  0x05,0x60,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// your dice (white)
PROGMEM const unsigned char d1[] = { 7,7,0xFE,0x82,0x82,0x92,0x82,0x82,0xFE };
PROGMEM const unsigned char d2[] = { 7,7,0xFE,0x82,0xA2,0x82,0x8A,0x82,0xFE };
PROGMEM const unsigned char d3[] = { 7,7,0xFE,0x82,0x86,0x92,0xC2,0x82,0xFE };
PROGMEM const unsigned char d4[] = { 7,7,0xFE,0x82,0xAA,0x82,0xAA,0x82,0xFE };
PROGMEM const unsigned char d5[] = { 7,7,0xFE,0xAA,0x82,0x92,0x82,0xAA,0xFE };
PROGMEM const unsigned char d6[] = { 7,7,0xFE,0xAA,0x82,0xAA,0x82,0xAA,0xFE };
const unsigned char* yd[] = {d1, d2, d3, d4, d5, d6};
// enemy's dice (black)
PROGMEM const unsigned char e1[] = { 7,7,0xFE,0xFE,0xFE,0xEE,0xFE,0xFE,0xFE };
PROGMEM const unsigned char e2[] = { 7,7,0xFE,0xBE,0xFE,0xFE,0xFE,0xFA,0xFE };
PROGMEM const unsigned char e3[] = { 7,7,0xFE,0xFA,0xFE,0xEE,0xFE,0xBE,0xFE };
PROGMEM const unsigned char e4[] = { 7,7,0xFE,0xBA,0xFE,0xFE,0xFE,0xBA,0xFE };
PROGMEM const unsigned char e5[] = { 7,7,0xFE,0xBA,0xFE,0xEE,0xFE,0xBA,0xFE };
PROGMEM const unsigned char e6[] = { 7,7,0xFE,0xBA,0xFE,0xBA,0xFE,0xBA,0xFE };
const unsigned char* ed[] = {e1, e2, e3, e4, e5, e6};

PROGMEM const unsigned char skull[] = { 9,9,0x7F,0x00,0xFF,0x80,0xDD,0x80,0x88,0x80,0xD8,0x80,0xFD,0x80,0xF7,0x80,0x7F,0x00,0x55,0x00 }; 

#ifdef FONT5X7_C
extern const byte font3x5[]; // a small but efficient font (default)
extern const byte font5x7[]; // a large, comfy font
#endif

#ifdef debug // declare dummy gamebook
PROGMEM const char dummy[]=
#ifdef FRENCH
"Avant de vous lancer dans cette aventure, il vous faut d'abord determiner vos propres forces et faiblesses. Vous avez en votre possession une epee et un bouclier, ainsi qu'un sac a dos contenant des provisions (nourriture et boissons) pour le voyage. Afin de vous preparer a votre quete, vous vous etes entraine au maniement de l'epee et vous vous etes exerce avec acharnement a accroitre votre endurance.\n"
"Pressez \026 pour ouvrir la feuille d'aventure.\n"
"Test paragraphe: allez au >2 ou au >123.\n"
"<Tentez votre Chance!\n"
"Combattez: ^Ennemi numero 1 H06/E05";
#else
"Before embarking on your adventure, you must first determine your own strengths and weaknesses. You have in your possession a sword and a shield together with a rucksack containing provisions (food and drink) for the trip. You have been preparing for your quest by training yourself in swordplay and exercising vigorously to build up your stamina.\n"
"Press \026 to open the adventure sheet.\n"
"Change paragraph: turn to >2 or to >123.\n"
"<Test your Luck!\n"
"Fight: ^Enemy number 1 s06/S05";
#endif // choice of language for dummy book
#endif // #ifdef debug

// stats: skill, stamina, luck (current/total) ; paragraph & other variables
byte skiC, skiT, staC, staT, luC, luT, potion, potionType, gold, prov;
word par, poffset, oldpoffset;
int crline, oldcrline;
byte nxline, nxpage, pcaret, oldpcaret;
boolean inAdvSheet=false;
int ldverr;
char eqp[EQPSIZE]; // Offers EQPSIZE-1 usable characters, from eqp[0] to eqp[EQPSIZE-2] + final '\0' at eqp[EQPSIZE-1]
char txt[TXTSIZE]; // Offers TXTSIZE-1 usable characters, from txt[0] to txt[TXTSIZE-2] + final '\0' at txt[TXTSIZE-1]

// Following two functions are needed by petit_fatfs
byte rx() { SPDR=0xFF; loop_until_bit_is_set(SPSR, SPIF); return SPDR; }
void tx(byte d) { SPDR=d; loop_until_bit_is_set(SPSR, SPIF); }

// Prototypes
void myTitleScreen(const __FlashStringHelper* title1, const __FlashStringHelper* title2, const uint8_t* logo);
void catStr(const __FlashStringHelper* nstr);
void catStr(char* nstr);
void catInt(int ni);
void showAdventureSheet();
byte nbOfEquipment();
byte itemOffset(byte nItm);
void showAbout();
boolean showDice(byte mode);
void reduceStat(byte &stat);
void printFString(boolean right, const char* str);
void printFStamina(boolean right, byte stat, boolean condition);
void showFight();
int char2int(char* str);
void int2char(int nb, char* str);
int numKeyboard(int init, byte digits);
void printAlpha(boolean caps, byte i);
void alphaKeyboard(char* tgt, byte lmax);
void changeStat(byte &stat);
long Toc(int idx);
void backToBook();
void readBook();
void seekBook(int tgtline);
void printBook();
void printCaret(boolean condition);
byte prevCaretOnScreen();
byte nextCaretOnScreen();

void moveCursor(byte x, byte y) {
  gb.display.cursorX+=
    (x>=CURRENT-NCHRX?x-CURRENT // relative CURRENT +/- n
    :x-gb.display.cursorX); // absolute
  gb.display.cursorY+=
    (y>=NEXTLINE-NCHRY?y-NEXTLINE+gb.display.fontHeight: // relative NEXTLINE +/- n
    (y>=PREVLINE-NCHRY?y-PREVLINE-gb.display.fontHeight: // relative PREVLINE +/- n
    (y>=CURRENT?y-CURRENT // relative CURRENT +/- n
    :y-gb.display.cursorY))); // absolute
}

void initGame() {
  potion=2; // measures of potion
  prov=10; // provisions
  skiC=skiT=random(1,7)+6; // 1D+6
  staC=staT=random(2,13)+12; // 2D+12
  luC=luT=random(1,7)+6; // 1D+6
  potionType=255; // 0=SKILL ; 1=STAMINA ; 2=LUCK ; 255=Undefined
  par=0;
  strcpy_P(eqp, DF_eqp_ini); // equipment
}

void restoreStatsFromEEPROM() {
  word id;
  id=(EEPROM.read(0) << 8) & 0xFF00; //MSB
  id+=EEPROM.read(1) & 0x00FF; //LSB
  if(id!=EEPROM_IDENTIFIER) return; // cancel if no or bad EEPROM identifier
  skiC=EEPROM.read(2);
  skiT=EEPROM.read(3);
  staC=EEPROM.read(4);
  staT=EEPROM.read(5);
  luC=EEPROM.read(6);
  luT=EEPROM.read(7);
  potion=EEPROM.read(8);
  potionType=EEPROM.read(9);
  gold=EEPROM.read(10);
  prov=EEPROM.read(11);
  par=(EEPROM.read(12) << 8) & 0xFF00; //MSB
  par+=EEPROM.read(13) & 0x00FF; //LSB
  for(byte i=0; i<EQPSIZE-1; i++) {
    eqp[i]=EEPROM.read(14+i);
  } eqp[EQPSIZE-1]=NULL;
}

void saveStatsToEEPROM() {
  EEPROM.write(0, (EEPROM_IDENTIFIER >> 8) & 0x00FF); //MSB
  EEPROM.write(1, EEPROM_IDENTIFIER & 0x00FF); //LSB
  EEPROM.write(2, skiC);
  EEPROM.write(3, skiT);
  EEPROM.write(4, staC);
  EEPROM.write(5, staT);
  EEPROM.write(6, luC);
  EEPROM.write(7, luT);
  EEPROM.write(8, potion);
  EEPROM.write(9, potionType);
  EEPROM.write(10, gold);
  EEPROM.write(11, prov);
  EEPROM.write(12, (par >> 8) & 0x00FF); //MSB
  EEPROM.write(13, par & 0x00FF); //LSB
  for(byte i=0; i<EQPSIZE-1; i++) {
    EEPROM.write(14+i, eqp[i]); }
}

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  gb.setFrameRate(10);
  #ifdef debug
  ldverr=FR_OK;
  #else
  Serial.begin(9600); PFFS.begin(10, rx, tx); // initialize petit_fatfs
  ldverr=pf_open(LDV);
  #endif
  initGame(); // initialize player stats
  restoreStatsFromEEPROM(); // restore saved stats
  myTitleScreen(PGMT(DF_title1), PGMT(DF_title2), DF_logo); // show title screen
  gb.pickRandomSeed();
} // setup()

void loop() {
  // put your main code here, to run repeatedly:
  if(gb.update()) {
    int choice;

    if(!staC && !inAdvSheet) { // no more STAMINA points
      gb.display.drawBitmap(37, 0, skull);
      gb.display.cursorY=10;
      gb.display.textWrap=true;
      #ifdef FRENCH
      gb.display.println(F("Vous etes mort..."));
      gb.display.println(F("Pressez \025 pour retenter l'aventure"));
      #else
      gb.display.println(F("You are dead..."));
      gb.display.println(F("Press \025 to restart the adventure"));
      #endif
      gb.display.textWrap=false;

    } else { // still in the game ;)
      gb.display.textWrap=true;
      printBook();
      gb.display.textWrap=false;

      // handle user input
      if(gb.buttons.pressed(BTN_DOWN)) { // DOWN
        if(nextCaretOnScreen()!=255)
          pcaret=nextCaretOnScreen();
        else if(nxline) { // we can scroll down
          crline++; // increase absolute current line by 1
          poffset+=nxline; // add relative offset to go to next line
          if(inAdvSheet) showAdventureSheet(); else readBook();
          if(pcaret!=255) { if(pcaret<nxline) pcaret=255; else pcaret-=nxline; }
        } // can scroll down
      } // DOWN

      if(gb.buttons.pressed(BTN_UP)) { // UP
        if(prevCaretOnScreen()!=255)
          pcaret=prevCaretOnScreen();
        else if(crline) { // we can scroll up
          if(inAdvSheet) {
            crline--; showAdventureSheet();
          } else seekBook(crline-1); // seek book to find offset for absolute current line minus 1
          pcaret=255; // reset caret for next printBook()
        } // can scroll up
      } // UP

      if(gb.buttons.pressed(BTN_RIGHT)) { // PAGE-DOWN
        if(nxpage) {
          pcaret=255; // reset caret for next printBook()
          crline+=NCHRY; // increase absolute current line by (number.of.lines.per.screen)
          poffset+=nxpage; // add relative offset to go to next page
          if(inAdvSheet) showAdventureSheet(); else readBook();
        }
      } // PAGE-DOWN

      if(gb.buttons.pressed(BTN_LEFT)) { // PAGE-UP
        if(crline) { // we can scroll up
          if(inAdvSheet) {
            crline=max(0, crline-NCHRY);
            showAdventureSheet();
          } else seekBook(crline-NCHRY); // seek book to find offset for absolute current line minus (nb.of.lines.per.scr)
          pcaret=255; // reset caret for next printBook()
        } // can scroll up
      } // PAGE-UP

    } // still in the game

    if(gb.buttons.pressed(BTN_A) && !staC) { initGame(); backToBook(); } // Restart adventure when out of stamina and pressing A

    if(gb.buttons.pressed(BTN_A) && pcaret!=255) { // ACTION
      if(inAdvSheet) { // [IN ADVENTURE SHEET]

        if(txt[pcaret]=='^') { // Remove equipment
          if(gb.menu(DF_menu4, 2)==0) {
            byte lng; char newitm[ITMSIZE]; // Offers ITMSIZE-1 usable characters, from newitm[0] to newitm[ITMSIZE-2] + final '\0' at newitm[ITMSIZE-1]
            for(lng=0; ; lng++) { newitm[lng]=txt[pcaret+lng]; if(txt[pcaret+lng]=='\n') { newitm[++lng]=NULL; break; } }
            char* start=strstr(eqp, newitm); // returns pointer to 1st occurence of newitm[] in eqp[]
            if(!start) { newitm[--lng]=NULL; start=strstr(eqp, newitm); } // case of last item in the list
            for(byte i=0; ; i++) {
              *(start+i)=*(start+lng+i);
              if(*(start+i)==NULL) break;
            }
            showAdventureSheet();
            pcaret=prevCaretOnScreen(); // because we changed text buffer so the existing caret was lost
          }

        } else if(txt[pcaret]=='>') switch(txt[pcaret+1]) {
          case '\001': case '\002': case '\003': // Change stat
            choice=gb.menu(DF_menu1, 3);
            if(choice<0 || choice==2) break; // cancelled by user
            switch(10*choice+(int)txt[pcaret+1]) {
              case 1: changeStat(skiC); break;
              case 2: changeStat(staC); break;
              case 3: changeStat(luC); break;
              case 11: changeStat(skiT); break;
              case 12: changeStat(staT); break;
              case 13: changeStat(luT); break;
            }
            if(skiC>skiT) skiC=skiT;
            if(staC>staT) staC=staT;
            if(luC>luT) luC=luT;
            showAdventureSheet();
            break;

          case '\004': // Gold
            choice=numKeyboard(gold, 3); // current gold as init number, 3 digits max
            if(choice!=-1) { gold=min(255, choice); showAdventureSheet(); }
            break;

          case '\005': // Provisions
            choice=gb.menu(DF_menu3, 3);
            if(choice==0 && prov>0) { // eat a meal
              prov--; staC+=4;
              if(staC>staT) staC=staT;
              gb.popup(DF_meal,15);
              showAdventureSheet();
            } else if(choice==1) { // add provisions
              choice=numKeyboard(prov, 2); // current provisions as init number, 2 digits max i.e. range 0-99
              if(choice!=-1) { prov=choice; showAdventureSheet(); }
            } // else it was cancelled by user
            break;

          case '\006': // Potion
            if(potionType==255) { // Choose potion at beginning of game
              choice=gb.menu(DF_menu5, 4);
              if(choice>=0 && choice <=2) potionType=choice;
            } else if(potion>0 && gb.menu(DF_menu2, 2)==0) { // Drink potion
              potion--;
              if(potionType==1) { staC=staT; gb.popup(DF_po_sta,15); }
              else if(potionType==2) { luT++; luC=luT; gb.popup(DF_po_luc,15); }
              else { skiC=skiT; gb.popup(DF_po_ski,15); }
            }
            showAdventureSheet();
            break;

          case '\011': // Save game
            saveStatsToEEPROM();
            inAdvSheet=false;
            backToBook();
            gb.popup(DF_savok,15);
            break;

          case '\012': // Roll 1 die
            showDice(ROLL_ONE_DIE);
            break;

          case '\013': // Roll 2 dice
            showDice(ROLL_BOTH_DICE);
            break;

          case '\014': // Goto paragraph
            choice=numKeyboard(-1, 3); // no init number, 3 digits max
            if(choice!=-1 && choice<=NPAR) { par=choice; inAdvSheet=false; backToBook(); }
            break;

          case '\015': // Add equipment
            char newitm[ITMSIZE]=""; // Offers ITMSIZE-1 usable characters, from newitm[0] to newitm[ITMSIZE-2] + final '\0' at newitm[ITMSIZE-1]
            alphaKeyboard(newitm, ITMSIZE-1); // -1 to leave room for final '\0'
            if(strlen(newitm) && strlen(eqp)<EQPSIZE-1) {
              if(strlen(eqp)) strcat(eqp, "\n"); strcat(eqp, "^");
              strncat(eqp, newitm, EQPSIZE-1-strlen(eqp)); // strncat instead of strcat to avoir buffer overflow
              eqp[EQPSIZE-1]=NULL; // final '\0' for protection
              showAdventureSheet();
              pcaret=nextCaretOnScreen(); // because we changed text buffer so the existing caret was lost
            }
            break;
        }

      } else { // [IN BOOK]
        if(txt[pcaret]=='>') { // Goto paragraph
          par=char2int(txt+pcaret+1);
          backToBook(); }
        else if(txt[pcaret]=='<')
          showDice(TEST_YOUR_LUCK);
        else if(txt[pcaret]=='^') {
          showFight(); readBook(); }
      } // [IN BOOK]
    } // ACTION

    if(gb.buttons.pressed(BTN_B)) { // ADVENTURE SHEET <-> BOOK
      inAdvSheet=!inAdvSheet;
      if(inAdvSheet) { // BOOK -> ADVENTURE SHEET
        oldpoffset=poffset; // save position in book
        oldcrline=crline;
        oldpcaret=pcaret;
        poffset=crline=0; pcaret=255; // display beginning of adventure sheet
        showAdventureSheet();
      } else { // ADVENTURE SHEET -> BOOK
        poffset=oldpoffset; // restore position in book
        crline=oldcrline;
        pcaret=oldpcaret;
        readBook();
      }
    }

    if(gb.buttons.pressed(BTN_C)) { // TITLE
      saveStatsToEEPROM();
      gb.popup(DF_savok,15);
      myTitleScreen(PGMT(DF_title1), PGMT(DF_title2), DF_logo);
    }

  } // if (gb.update())
} // loop()

void changeStat(byte &stat) {
  byte newstat = numKeyboard(stat, 2); // current stat as init number, 2 digits max i.e. range 0-99
  if(newstat!=-1) stat=newstat;
}

long Toc(int idx) { // read paragraph offset in Table of Content on SD
  WORD br; char tmp[4]; // Offers 3 usable characters, from eqp[0] to eqp[2] + final '\0' at eqp[3]
  pf_lseek(3*idx); // go in ToC
  pf_read((void*)&tmp[0], 3, &br); // read 3 bytes
  return 0x10000L*(byte)tmp[0]+0x100L*(byte)tmp[1]+(byte)tmp[2];
}

void showToc() {
  while(1) {
    if(gb.update()) {
      gb.display.print(F("par: ")); gb.display.println(par);
      gb.display.print(F("Toc: 0x"));
      WORD br; char tmp[5]; // Offers 3 usable characters, from eqp[0] to eqp[2] + final '\0' at eqp[3]
      pf_lseek(3*par); // go in ToC
      pf_read((void*)&tmp[0], 3, &br); // read 3 bytes
      for(byte i=0; i<3; i++) { gb.display.print((byte)tmp[i], HEX); }
      gb.display.println("");
      gb.display.print(F("decimal: "));
      gb.display.println(Toc(par));
      if(gb.buttons.pressed(BTN_B)) break;
    }
  }
}

void backToBook() {
  poffset=crline=0;
  pcaret=255;
  readBook();
}

void readBook() { // read current paragraph Toc(par) at poffset and store into txt[] (buffer the size of the screen)
  #ifdef debug // use dummy gamebook
  int psize=min(TXTSIZE-2, strlen(dummy)-poffset);
  strncpy_P(&txt[0], dummy+poffset, psize);
  txt[psize+1]=NULL;
  #else        // open real gamebook on SD
  //showToc();
  WORD br;
  long crpoffset=Toc(par)+poffset;
  long nxpoffset=Toc(par+1);
  int psize=min(TXTSIZE-2, nxpoffset-crpoffset);
  pf_lseek(crpoffset); // go to correct offset
  pf_read((void*)&txt[0], psize, &br);
  txt[psize+1]=NULL;
  #endif
}

void seekBook(int tgtline) { // seek current paragraph for target line (used when reading up w/ buttons UP/LEFT)
  byte x,y,nxl[11]; // nxl[] index starts at 0 for first new line (=2nd line on screen) up to (NCHRY-2)+1 for next page offset
  poffset=0; crline=0; // start from the top of the paragraph
  if(tgtline<=0) { readBook(); return; } // very first line wanted -> we're done (+can't seek before beginning of paragraph)
  do {
    readBook();
    x=1; y=1;
    for(byte i=0; ; i++) { // simulate printing txt[] on screen + find all offsets to new lines (nxl[])
      char c=txt[i];
      if(c==NULL || y>NCHRY) break; // screen has been filled
      if(x>1 || c!='\n') { x++; if(x>NCHRX || c=='\n') { y++; x=1; nxl[y-2]=i+1; } }
    } // for(byte i=0; ; i++)
    if(tgtline!=crline) {
      if(tgtline<=crline+NCHRY) { // target line is somewhere in current page
        poffset+=nxl[tgtline-crline-1];
        crline=tgtline; // found!
        readBook();
      } else { // target line is in another page -> proceed to load next one
          crline+=NCHRY; // go down 1 page
          poffset+=nxl[NCHRY-1]; // prepare offset for next iteration of while() loop
      }
    } // if(tgtline!=crline)
  } while(crline!=tgtline);
}

void printBook() { // print txt[] on screen (current paragraph at poffset) + set next line (nxline) and next page (nxpage) offsets
  nxline=0; nxpage=0;
  for(byte i=0; ; i++) {
    char c=txt[i];
    if(c==NULL) { nxline=0; return; }
    if(gb.display.cursorY>=LCDHEIGHT) { nxpage=i; return; }
    if(C_IS_SPECIAL_CHARACTER) {
      if(pcaret==255) pcaret=i;
      printCaret(pcaret==i && (gb.frameCount/4)%2);
      if(inAdvSheet && c=='>') i++; // skip special code following caret in adventure sheet
    } else if(gb.display.cursorX || c!='\n') gb.display.print(c);
    if(!nxline && gb.display.cursorY>0) nxline=i+1;
  }
}

void printCaret(boolean condition) {
  if(condition && (gb.frameCount/4)%2) gb.display.print(F("\020")); else gb.display.print(F("\032"));
}

byte prevCaretOnScreen() {
  if(pcaret==255) return 255;
  for(byte i=pcaret-1; ; i--) {
    if(i==255) break;
    char c=txt[i];
    if(C_IS_SPECIAL_CHARACTER) return i;
  }
  return 255;
}

byte nextCaretOnScreen() {
  if(pcaret==255) return 255;
  for(byte i=pcaret+1; ; i++) {
    char c=txt[i];
    if(i==nxpage || c==NULL) break;
    if(C_IS_SPECIAL_CHARACTER) return i;
  }
  return 255;
}

