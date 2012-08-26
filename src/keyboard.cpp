#include "keyboard.h"

Keyboard::Keyboard()
{
  keyString = (char *)malloc( 20 * sizeof(char));
  PrintUp  =FALSE;
  loadKeyCode();
}

void Keyboard::loadKeyCode()
{
  QCoreApplication::setOrganizationName( ORG_NAME );
  QCoreApplication::setOrganizationDomain( ORG_DOMAIN );
  QCoreApplication::setApplicationName( APP_NAME );

  QSettings settings;

  settings.beginGroup( "Main" );
  keyCode = settings.value( "keycode", 65).toInt();
  settings.endGroup();
 }

bool Keyboard::start()
{
  disp = XOpenDisplay(NULL);
  screen = DefaultScreen(disp);

  if( disp == NULL )
    return false;

  //x11_fd = ConnectionNumber(disp);
  //XFlush(disp);
  grabbed = false;

  //snoop();
  XSynchronize(disp, TRUE);

  /* setup buffers */
  saved=buf1;
  keys=buf2;
  XQueryKeymap(disp, saved);

  return true;
}

void Keyboard::snoop()
{
  snoop_all_windows(DefaultRootWindow(disp), KeyPressMask);
}

void Keyboard::noSnoop()
{
  snoop_all_windows(DefaultRootWindow(disp), NoEventMask);
}

void Keyboard::flush()
{
  XFlush(disp);
}

Display *Keyboard::getDisplay()
{
  return disp;
}

void Keyboard::snoop_all_windows(Window root, unsigned long type)
{
  static int level = 0;
  Window parent, *children;
  unsigned int i, nchildren;
  int stat;

  level++;

  stat = XQueryTree(disp, root, &root, &parent, &children, &nchildren);
  if (stat == FALSE)
   {
     fprintf(stderr, "Can't query window tree...\n");
     return;
   }

  if (nchildren == 0)
    return;

  XSelectInput(disp, root, type);

  for(i=0; i < nchildren; i++)
   {
     XSelectInput(disp, children[i], type);
     snoop_all_windows(children[i], type);
   }

  XFree((char *)children);
}

#define KEY_BUFF_SIZE 256
static char key_buff[KEY_BUFF_SIZE];

char *Keyboard::TranslateKeyCode(XEvent *ev)
{
  int count;
  char *tmp;
  KeySym ks;

  if (ev)
   {
     count = XLookupString((XKeyEvent *)ev, key_buff, KEY_BUFF_SIZE, &ks,NULL);
     key_buff[count] = '\0';

     if (count == 0)
      {
        tmp = XKeysymToString(ks);
        if (tmp)
          strcpy(key_buff, tmp);
        else
          strcpy(key_buff, "");
      }

     return key_buff;
   }
  else
    return NULL;
}

/* Return 0:No event, 1:Key event, 2:Scape */
unsigned int Keyboard::grabEvent()
{
  /* find changed keys */
  XQueryKeymap(disp, keys);
  int event = 0;

  for (i=0; i<32*8; i++) {
     if (BIT(keys, i)!=BIT(saved, i)) {
        register char *str;
        str=(char *)KeyCodeToStr(i, BIT(keys, i), KeyModifiers(keys));
        if (BIT(keys, i)!=0 || PrintUp){
          if(i==keyCode){
            event++;
          }
        };
        fflush(stdout); /* in case user is writing to a pipe */
     }
  }

  /* swap buffers */
  char_ptr=saved;
  saved=keys;
  keys=char_ptr;
  if(event>0)
    return 1;
  else
    return 0;

  //usleep(delay);
}

/* This part takes the keycode and makes an output string. */

/*
   Have a keycode, Look up keysym for it.
   Convert keysym into its string representation.
   if string is more than one character try to reduce it to one.
   if string still is more than one character, put it into the form
   (+string) or (-string) depending on whether the key is up or down.
   print out the string.
*/

struct conv {char from[20], to[5];} convertTable[] = {
   /* shift & control replaced with nothing, since they are appearent
      from the output */
   {"return",""},    {"escape","^["},    {"delete", "^H"},
   {"shift",""},       {"control",""},     {"tab","\t"},
   {"space", " "},     {"exclam", "!"},    {"quotedbl", "\""},
   {"numbersign", "#"},{"dollar", "$"},    {"percent", "%"},
   {"ampersand", "&"}, {"apostrophe", "'"},{"parenleft", "("},
   {"parenright", ")"},{"asterisk", "*"},  {"plus", "+"},
   {"comma", ","},     {"minus", "-"},     {"period", "."},
   {"slash", "/"},     {"colon", ":"},     {"semicolon", ";"},
   {"less", "<"},      {"equal", "="},     {"greater", ">"},
   {"question", "?"},  {"at", "@"},        {"bracketleft", "["},
   {"backslash", "\\"},{"bracketright", "]"},{"asciicircum", "^"},
   {"underscore", "_"},{"grave", "`"},     {"braceleft", "{"},
   {"bar", "|"},       {"braceright", "}"},{"asciitilde", "~"},
   {"odiaeresis","ö"},{"udiaeresis","ü"},{"adiaeresis","ä"},{"",""}
};

int Keyboard::StrToChar(char *data) {
   int i=0;
   while (convertTable[i].from[0]!=0 || convertTable[i].to[0]!=0) {
      if (!strncasecmp(data,convertTable[i].from,
                       strlen(convertTable[i].from)) ) {
         strcpy(data,  convertTable[i].to);
         return TRUE;
      }
      i++;
   }
   return FALSE;
}

char *Keyboard::KeyCodeToStr(int code, int down, int mod) {
   static char *str, buf[KEYSYM_STRLEN+1];
   int index;
   KeySym  keysym;
   /* get the keysym for the appropriate case */
  switch (mod) {
    case SHIFT_DOWN:
      index=SHIFT_INDEX;
      break;
    case ISO3_DOWN:
      index=ISO3_INDEX;
      break;
    case MODE_DOWN:
      index=MODE_INDEX;
      break;
    default:
      index=0;
  }

   keysym=XKeycodeToKeysym(disp, code, index);
   if (NoSymbol==keysym) return "";

   /* convert keysym to a string, copy it to a local area */
   str=XKeysymToString(keysym);

   if (strcmp(str,"ISO_Level3_Shift") == 0) {
    keysym=XKeycodeToKeysym(disp, code, ISO3_INDEX);
    str=XKeysymToString(keysym);
   }

   if (NULL==str) return "";
   strncpy(buf, str, KEYSYM_STRLEN); buf[KEYSYM_STRLEN]=0;

   /* try to reduce strings to character equivalents */
   if (buf[1]!=0 && !StrToChar(buf)) {
  if (strcmp(buf, "Caps_Lock") == 0) return "";
      /* still a string, so put it in form (+str) or (-str) */
      if (down) strcpy(buf, "(+");
      else      strcpy(buf, "(-");
      strcat(buf, str);
      strcat(buf, ")");
      return buf;
   }
   if (buf[0]==0) return "";
   if (mod==CONTROL_DOWN) {
      buf[2]=0;
      buf[1]=toupper(buf[0]);
      buf[0]='^';
   }
   if (mod==LOCK_DOWN) {buf[0]=toupper(buf[0]);}
   return buf;
}

/* returns which modifier is down, shift/caps or control */
int Keyboard::KeyModifiers(char *keys) {
  static int setup=TRUE;
  static int width;
  static XModifierKeymap *mmap;
  int i;

  if (setup) {
    setup=FALSE;
    mmap=XGetModifierMapping(disp);
    width=mmap->max_keypermod;
  }
  for (i=0; i<width; i++) {
    KeyCode code;

    code=mmap->modifiermap[ControlMapIndex*width+i];
    if (code && 0!=BIT(keys, code)) {return CONTROL_DOWN;}

    code=mmap->modifiermap[ShiftMapIndex*width  +i];
    if (code && 0!=BIT(keys, code)) {return SHIFT_DOWN;}

    code=mmap->modifiermap[LockMapIndex*width   +i];
    if (code && 0!=BIT(keys, code)) {return LOCK_DOWN;}

    code=mmap->modifiermap[Mod3MapIndex*width   +i];
    if (code && 0!=BIT(keys, code)) {return ISO3_DOWN;}

    code=mmap->modifiermap[Mod5MapIndex*width   +i];
    if (code && 0!=BIT(keys, code)) {return MODE_DOWN;}
  }
  return 0;
}

void Keyboard::stop()
{
  XCloseDisplay(disp);
}

void Keyboard::setEscapeCode( int i )
{
  escapeCode = i;
}

void Keyboard::setKeyCode( int i )
{
  keyCode = i;
}

void Keyboard::move( int x, int y )
{
  XTestFakeMotionEvent( disp, screen, x, y, 10 );
}

void Keyboard::key( int k )
{
  XTestFakeKeyEvent( disp, k, True, 0 );
  XTestFakeKeyEvent( disp, k, False, KEYPRESSTIME );
}

void Keyboard::click(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME );
}

void Keyboard::doubleClick(void)
{
  XTestFakeButtonEvent( disp, 1, True, KEYPRESSTIME2 );
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME2 );
  XTestFakeButtonEvent( disp, 1, True, KEYPRESSTIME2 );
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME2 );
}

void Keyboard::rightClick(void)
{
  XTestFakeButtonEvent( disp, 3, True, 0 );
  XTestFakeButtonEvent( disp, 3, False, KEYPRESSTIME );
}

void Keyboard::drag(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
}

void Keyboard::drop(void)
{
  XTestFakeButtonEvent( disp, 1, False, KEYPRESSTIME );
}

Keyboard::~Keyboard()
{
}
