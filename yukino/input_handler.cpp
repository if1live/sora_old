// Ŭnicode please
#include "yukino_stdafx.h"
#include "input_handler.h"

using namespace std;

namespace yukino {;
InputHandler::InputHandler() {
  SR_ASSERT(last_handler_ == NULL);
  last_handler_ = this;
}
InputHandler::~InputHandler() {
  SR_ASSERT(last_handler_ != NULL);
  last_handler_ = NULL;
}
InputHandler *InputHandler::last_handler_ = NULL;
InputHandler *InputHandler::GetLast() {
  return last_handler_;
}	
}
