#pragma once

#include <memory>
#include "graphics.hpp"
#include "window.hpp"

class Console {
 public:
  int kRows = 34, kColumns = 100;

  Console(const PixelColor& fg_color, const PixelColor& bg_color);
  void PutString(const char* s);
  void SetWriter(PixelWriter* writer);
  void SetWindow(const std::shared_ptr<Window>& window);
  void SetLayerID(unsigned int layer_id);
  unsigned int LayerID() const;

 private:
  void Newcolumn();
  void Newline();
  void Cursordelete();
  void Refresh();

  PixelWriter* writer_;
  std::shared_ptr<Window> window_;
  const PixelColor fg_color_, bg_color_;
  char buffer_[1000][1000];
  int cursor_row_, cursor_column_;
  unsigned int layer_id_;
};

extern Console* console;

void InitializeConsole();
