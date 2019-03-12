#define SCREEN_SCENE_CHOOSE 1

class Screen : public Adafruit_SH1106 {
	 OLED;
public:
	Screen (Adafruit_SH1106);
	/*template <class OLED>
	void setScene (OLED& display, String name) {
		display.println(name);
	}*/
};

Screen::Screen (Adafruit_SH1106 obj) {
	OLED = obj;
}