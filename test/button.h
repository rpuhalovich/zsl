#ifndef BUTTON_H
#define BUTTON_H

#include <ztypes.h>

FORWARD(sString, String)
FORWARD(sResult_bool, Result_bool)

typedef struct sButton {
    u32 id;
    String* string;
} Button;

Result_bool handlePress(Button* button);

#endif // BUTTON_H
