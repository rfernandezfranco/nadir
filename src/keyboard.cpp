#include "keyboard.h"

Keyboard::Keyboard()
{
  printUp = false;
  loadKeyCode();
}

void Keyboard::loadKeyCode()
{
  QCoreApplication::setOrganizationName( ORGANIZATION_NAME );
  QCoreApplication::setOrganizationDomain( ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName( APPLICATION_NAME );

  QSettings settings;

  settings.beginGroup( "Main" );
  keyCode = settings.value( "keycode", 65).toInt();
  settings.endGroup();
}

bool Keyboard::start()
{
  disp = XOpenDisplay(nullptr);
  if (!disp)
    return false;
  screen = DefaultScreen(disp);

  //XSynchronize(disp, TRUE) ensures events are processed sequentially
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
  Window parent, *children;
  unsigned int nchildren;
  int stat;

  stat = XQueryTree(disp, root, &root, &parent, &children, &nchildren);
  if (stat == FALSE)
   {
     fprintf(stderr, "Can't query window tree...\n");
     return;
   }

  if (nchildren == 0)
    return;

  XSelectInput(disp, root, type);

  for (unsigned int idx = 0; idx < nchildren; ++idx) {
    XSelectInput(disp, children[idx], type);
    snoop_all_windows(children[idx], type);
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

/* Return 0:No event, 1:Key event */
unsigned int Keyboard::grabKeyEvent()
{
  /* find changed keys */
  XQueryKeymap(disp, keys);
  int event = 0;

  for (int idx = 0; idx < 32 * 8; ++idx) {
    if (bit(keys, idx) != bit(saved, idx)) {
      if (bit(keys, idx) != 0 || printUp) {
        if (idx == keyCode) {
          event++;
        }
      }
      fflush(stdout); /* in case user is writing to a pipe */
    }
  }

  /* swap buffers */
  char *tmp = saved;
  saved = keys;
  keys = tmp;
  return event>0 ? 1 : 0;
}

/* Return 0:No event, 1:Button event */


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

const char *Keyboard::KeyCodeToStr(int code, int down, int mod) {
   static const char *str;
   static char buf[KEYSYM_STRLEN+1];
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
  static bool setup = true;
  static int width;
  static XModifierKeymap *mmap;

  if (setup) {
    setup = false;
    mmap = XGetModifierMapping(disp);
    width = mmap->max_keypermod;
  }
  for (int idx = 0; idx < width; ++idx) {
    KeyCode code;

    code = mmap->modifiermap[ControlMapIndex * width + idx];
    if (code && bit(keys, code)) { return CONTROL_DOWN; }

    code = mmap->modifiermap[ShiftMapIndex * width + idx];
    if (code && bit(keys, code)) { return SHIFT_DOWN; }

    code = mmap->modifiermap[LockMapIndex * width + idx];
    if (code && bit(keys, code)) { return LOCK_DOWN; }

    code = mmap->modifiermap[Mod3MapIndex * width + idx];
    if (code && bit(keys, code)) { return ISO3_DOWN; }

    code = mmap->modifiermap[Mod5MapIndex * width + idx];
    if (code && bit(keys, code)) { return MODE_DOWN; }
  }
  return 0;
}

void Keyboard::stop()
{
  if (disp) {
    XCloseDisplay(disp);
    disp = nullptr;
  }
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
  XTestFakeKeyEvent( disp, k, False, KEY_PRESS_TIME );
}

void Keyboard::click(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
  XTestFakeButtonEvent( disp, 1, False, KEY_PRESS_TIME );
}

void Keyboard::doubleClick(void)
{
  XTestFakeButtonEvent( disp, 1, True, KEY_PRESS_TIME2 );
  XTestFakeButtonEvent( disp, 1, False, KEY_PRESS_TIME2 );
  XTestFakeButtonEvent( disp, 1, True, KEY_PRESS_TIME2 );
  XTestFakeButtonEvent( disp, 1, False, KEY_PRESS_TIME2 );
}

void Keyboard::rightClick(void)
{
  XTestFakeButtonEvent( disp, 3, True, 0 );
  XTestFakeButtonEvent( disp, 3, False, KEY_PRESS_TIME );
}

void Keyboard::drag(void)
{
  XTestFakeButtonEvent( disp, 1, True, 0 );
}

void Keyboard::drop(void)
{
  XTestFakeButtonEvent( disp, 1, False, KEY_PRESS_TIME );
}

Keyboard::~Keyboard()
{
  stop();
}
