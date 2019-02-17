void SSD1316_Init(void) ;
void SSD1316_drawPixel(int16_t x, int16_t y, uint16_t color);
void SSD1316_Refresh(void);
void SSD1316_Page_write(uint8_t page, uint8_t *data);
void SSD1316_reg_write(uint8_t len, uint8_t *reg);
void SSD1316_Clear(void);
void SSD1316_screen_save(int on);