
//#pragma comment(lib,"Msimg32.lib"); // AlphaBlend
typedef int DINT;
typedef signed int SINT;
typedef long int LINT;
typedef long long int SLINT;
typedef long double LBLE;
typedef unsigned long int ULINT;

#define ENGINE_TEXT_MAX_LENGTH 96

#include <Windows.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include <string>

#include "../assets/engine.literary.h"


// Modify paths accordingly
#ifndef ENGINE_MAP_DIRECTORY_SOURCES
#define ENGINE_MAP_DIRECTORY_SOURCES			""
#endif

#ifndef ENGINE_MAP_DIRECTORY_SPRITE
#define ENGINE_MAP_DIRECTORY_SPRITE				"sprites"
#endif

#ifndef ENGINE_MAP_DIRECTORY_SPRITE_OUTPUT
#define ENGINE_MAP_DIRECTORY_SPRITE_OUTPUT		"sprites"
#endif

#ifndef ENGINE_MAP_DIRECTORY_BRACKETS
#define ENGINE_MAP_DIRECTORY_BRACKETS			"sprites2"
#endif 

#ifndef ENGINE_MAP_DIRECTORY_BRACKETS_OUTPUT
#define ENGINE_MAP_DIRECTORY_BRACKETS_OUTPUT	"sprites2"
#endif 

#ifndef ENGINE_MAP_DIRECTORY_POLYGON
#define ENGINE_MAP_DIRECTORY_POLYGON			"polygons"
#endif 

#ifndef ENGINE_VERSION_MINOR
#define ENGINE_VERSION_MINOR				0
#endif 

#ifndef ENGINE_VERSION_LESSER
#define ENGINE_VERSION_LESSER				0
#endif

#ifndef ENGINE_VERSION_LARGE
#define ENGINE_VERSION_LARGE				0
#endif

#ifndef ENGINE_VERSION_MAJOR
#define ENGINE_VERSION_MAJOR				0
#endif

#ifndef BACKGROUND_COLOR
#define ENGINE_BACKGROUND DGR
#else
#define ENGINE_BACKGROUND BACKGROUND_COLOR
#endif

#define MENU_VIEW GR

#ifndef ENGINE_MAP_BASE_POOL
#define ENGINE_MAP_BASE_POOL				{}
#endif
#ifndef ENGINE_MAP_PREFIX_POOL
#define ENGINE_MAP_PREFIX_POOL				{}
#endif
#ifndef ENGINE_MAP_SUFFIX_POOL
#define ENGINE_MAP_SUFFIX_POOL				{}
#endif;



namespace drx {
	namespace enums {
		enum class MASH { DOMINANT = 1, HALF = 2, INFERIOR = 3, MINUS = 4, PLUS = 5, RED = 6, GREEN = 7, BLUE = 8 };
		enum class DIR { ASC = 1, DESC = -1 };
		enum class DIRECTION { LEFT = 4, RIGHT = 6, UP = 8, DOWN = 2, HALT = 5, CENTER = 15 };
	}
	namespace utils {
		struct DICE {
			DICE() { };
			SINT _roll(SINT min, SINT max) {
				max++;
				DINT c = 0;
				do {
					min++;
					max++;
					c++;
				} while (min < 0);
				double r = (double)min + ((double)rand() / (double)RAND_MAX) * (double(max) - (double)min);
				SINT result = (SINT)r - 1 * c;
				return result;
			}

			double _droll(double min, double max) {
				return min + (((double)rand() / (double)RAND_MAX) * (max - min));
			}

			DINT _percent(DINT number, DINT percent = 100) {
				if ((rand() % percent) < number) {
					return 1;
				}
				else {
					return 0;
				}
			}

		};
		struct STRING {
			STRING() {
				this->length = 0;
				this->wtext[this->length] = L'\0';
				this->utext[this->length] = (unsigned char)'\0';
				this->text[this->length] = '\0';
			};
			STRING(const char text[]) {
				this->length = 0;
				this->wtext[this->length] = L'\0';
				this->utext[this->length] = (unsigned char)'\0';
				this->text[this->length] = '\0';
				this->_write(text);
			}
			STRING(const std::string string) {
				this->length = 0;
				this->wtext[this->length] = L'\0';
				this->utext[this->length] = (unsigned char)'\0';
				this->text[this->length] = '\0';
				this->_write(string.c_str());
			}
			~STRING() {
				this->length = 0;
				*this->wtext = {};
				*this->text = {};
				*this->utext = {};
			};
			DINT length;
			wchar_t wtext[ENGINE_TEXT_MAX_LENGTH];
			char text[ENGINE_TEXT_MAX_LENGTH];
			unsigned char utext[ENGINE_TEXT_MAX_LENGTH];
			drx::utils::DICE dice;

			void operator += (const char text[]) {
				this->_append(text);
			}

			void operator += (const DINT number) {
				drx::utils::STRING string = std::to_string(number);
				for (DINT i = 0; i < string.length; i++) {
					this->_app(string[i]);
				}

			}

			void operator += (const double number) {
				drx::utils::STRING s = std::to_string(number);
				for (DINT i = 0; i < s.length; i++) {
					this->_app(s[i]);
				}
			}

			void operator += (const std::string string) {
				this->_append(string.c_str());
			}

			void operator << (const char chr) {
				this->text[this->length] = chr;
				this->length++;
			}

			operator STRING() {
				return this->text;
			}

			char operator[] (DINT position) {
				return this->text[position];
			}


			bool _match(const char text[]) {
				DINT t = 0;
				drx::utils::STRING n = text;
				for (DINT c = 0; c < this->text[c] != '\0'; c++) {
					char cursor = this->text[c];
					if (cursor != text[t]) {
						t = 0;
					}
					else {
						t++;
					}
					if (t == n.length) return true;
				}
				return false;
			}

			bool _exact(const char text[]) {
				drx::utils::STRING t = text;
				if (t.length != this->length) return false;
				for (DINT c = 0; c < this->text[c] != '\0'; c++) {
					char cursor = this->text[c];
					if (cursor != text[c]) return false;
				}
				return true;
			}

			void _slice(DINT b, DINT e) {
				drx::utils::STRING sliced;
				for (DINT c = 0; this->text[c] != '\0'; c++) {
					char cursor = this->text[c];
					if (c < b) sliced._app(cursor);
					if (c > e) sliced._app(cursor);
				}
				this->_write(sliced.text);
			}

			bool _strip(char a, char b) {
				drx::utils::STRING stripped;
				bool bFound = false, eFound = false;
				for (DINT c = 0; this->text[c] != '\0'; c++) {
					char cursor = this->text[c];
					//std::cout << cursor;
					if (eFound) {
						stripped._app(cursor);
					}
					else {
						if (!bFound) {
							if (cursor == a) bFound = true;
						}
						else {
							if (cursor == b) eFound = true;
						}
					}
				}
				if (eFound) this->_write(stripped.text);
				return eFound;
			}


			void _replace(char a, char b) {
				for (DINT c = 0; c < this->length; c++) {
					if (this->text[c] == a) this->text[c] = b;
				}
			}

			DINT _has(char a) {
				for (DINT c = 0; c < this->length; c++) {
					if (this->text[c] == a) return 1;
				}
				return 0;
			}

			void _generate(DINT length = 5) {
				if (length > ENGINE_TEXT_MAX_LENGTH - 1) length = ENGINE_TEXT_MAX_LENGTH - 1;
				this->_clear();
				for (; length > this->length;) {
					switch (this->dice._roll(0, 8)) {
					default:
						this->_append("A");
						break;
					case 0:
						this->_append("No");
						break;
					case 1:
						this->_append("Ka");
						break;
					case 2:
						this->_append("o");
						break;
					case 3:
						this->_append("V");
						break;
					case 4:
						this->_append("de");
						break;
					case 5:
						this->_append("r");
						break;
					case 6:
						this->_append("qu");
						break;
					case 7:
						this->_append("s");
						break;
					}
				}
				this->text[length] = '\0';
			}

			void _append(const char text[]) {
				for (DINT i = 0; text[i] != '\0'; i++) {
					this->text[this->length] = text[i];
					this->wtext[this->length] = (wchar_t)text[i];
					this->length++;
				}
				this->text[this->length] = '\0';
				this->wtext[this->length] = L'\0';
			}

			void _app(const char character) {
				this->text[this->length] = character;
				this->length++;
				this->text[this->length] = '\0';
			}

			void _space(const char text[]) {
				if (this->length != 0) {
					this->text[this->length] = ' ';
					this->wtext[this->length] = L' ';
					this->length++;
				}
				this->_append(text);
			}

			void _add(char character) {
				this->text[this->length] = character;
				this->length++;
			}

			void _erase() {
				this->~STRING();
				*this = {};
			}

			void _clear() {
				if (this->length > 0) {
					for (; this->length > 0; this->length--) {
						this->text[this->length - 1] = '\0';
						this->utext[this->length - 1] = (unsigned char)'\0';
						this->wtext[this->length - 1] = L'\0';
					}
				}
			}

			void _reverse() {
				drx::utils::STRING temp;
				temp._clear();
				for (temp.length = 0; temp.length < this->length; temp.length++) {
					temp.wtext[temp.length] = this->wtext[this->length - temp.length - 1];
					temp.text[temp.length] = this->text[this->length - temp.length - 1];
					temp.utext[temp.length] = this->utext[this->length - temp.length - 1];
				}
				*this = temp;


			}

			void _write(const char text[]) {
				this->_clear();
				for (this->length = 0; text[this->length] != '\0'; this->length++) {
					this->text[this->length] = text[this->length];
					this->wtext[this->length] = (wchar_t)text[this->length];
					this->utext[this->length] = (unsigned char)text[this->length];
				}
				this->text[this->length] = '\0';
				this->wtext[this->length] = L'\0';
				this->utext[this->length] = (unsigned char)'\0';
			}

			void _uwrite(unsigned char text[]) {
				this->_clear();
				for (this->length = 0; text[this->length] != '\0'; this->length++) {
					this->text[this->length] = (char)text[this->length];
					this->wtext[this->length] = (wchar_t)text[this->length];
					this->utext[this->length] = text[this->length];
				}
				this->text[this->length] = '\0';
			}

			void _wwrite(const wchar_t text[]) {
				this->_clear();
				for (this->length = 0; text[this->length] != '\0'; this->length++) {
					this->text[this->length] = (char)text[this->length];
					this->wtext[this->length] = text[this->length];
					this->utext[this->length] = (unsigned char)text[this->length];
				}
				this->text[this->length] = '\0';
			}

			void _prev() {
				if (this->length > 0) this->text[this->length - 1] = '\0';
				if (this->length > 0) this->length--;
			}

			void _dump() {
				std::cout << "\n>STRING: " << this->text;
			}
		};
		struct SPOT {
			struct COLOR {
				COLOR(DINT r = 0, DINT g = 0, DINT b = 0) {
					this->red = r;
					this->green = g;
					this->blue = b;
				}
				DINT red, green, blue;
			};
			SPOT(double x = 0.0, double y = 0.0, double z = 0.0, drx::utils::SPOT::COLOR color = { 0, 0, 0 }) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->color = color;
				this->locked = false;
				this->max = 0.0;
				this->min = 0.0;
			};
			~SPOT() {};
			double x, y, z, min, max;
			bool locked;
			drx::utils::SPOT::COLOR color;
			bool operator == (drx::utils::SPOT b) {
				if (this->x == b.x && this->y == b.y && this->z == b.z) return true;
				return false;
			}

			void _dump() {
				std::cout << "\n> X " << this->x << ", Y " << this->y << ", Z " << this->z;
				std::cout << " ( " << this->color.red << ", " << this->color.green << ", " << this->color.blue << " )";
			}

			bool _square(double r, drx::utils::SPOT c) {
				double x = this->x - r;
				double y = this->y - r;
				double w = this->x + r;
				double h = this->y + r;

				return (x <= c.x && w >= c.x && y <= c.y && h >= c.y);
			}

			bool _round(double r, drx::utils::SPOT b) {
				double x = b.x - this->x;
				double y = b.y - this->y;
				return sqrt(x * x + y * y) <= r;
			}

		};
		struct SSPOT : SPOT {
			SSPOT(double x = 0.0, double y = 0.0, double w = 0.0, double h = 0.0, double z = 0.0, double d = 0.0) {
				this->x = x;
				this->y = y;
				this->w = w;
				this->h = h;
				this->z = z;
				this->d = d;
			};
			SSPOT(bool inf) {
				if (inf) {
					this->x = INFINITY;
					this->y = INFINITY;
					this->z = INFINITY;
					this->w = -INFINITY;
					this->h = -INFINITY;
					this->d = -INFINITY;
				}
				else {
					this->x = 0.0;
					this->y = 0.0;
					this->z = 0.0;
					this->w = 0.0;
					this->h = 0.0;
					this->d = 0.0;
				}
			};
			double w, h, d;


			void _dump() {
				std::cout << "\nSSPOT: " << this->x << " (" << this->w << "), " << this->y << " (" << this->h << "), " << this->z << " (" << this->d << ")";
			}

			bool _within(drx::utils::SPOT s) {
				return (s.x >= this->x && s.x <= this->w && s.y >= this->y && s.y <= this->h);
			}
		};
		struct DIMENSION {
			DIMENSION(DINT x = 0, DINT y = 0) {
				this->x = x;
				this->y = y;
				this->w = 0;
				this->h = 0;
				this->d = 0;
				this->size = 0;
				this->direction = 0;
				this->id = -1;
			}
			DIMENSION(DINT x, DINT y, DINT w, DINT h, DINT d = 0) {
				this->x = x;
				this->y = y;
				this->w = w;
				this->h = h;
				this->d = d;
				this->size = 1;
				this->direction = 0;
				this->id = -1;
			}
			DIMENSION(SSPOT s) {
				this->x = (DINT)s.x;
				this->y = (DINT)s.y;
				this->w = (DINT)s.w;
				this->h = (DINT)s.h;
				this->d = (DINT)s.d;
				this->size = 0;
				this->direction = 0;
				this->id = -1;
			}
			DINT x, y, w, h, d, size, direction;
			SINT id;

			DINT _within(DINT x, DINT y) {
				return (x >= this->x && x <= this->w + this->x && y >= this->y && y <= this->h + this->y);
			}
			void _set(drx::utils::DIMENSION dimension) {
				this->x = dimension.x;
				this->y = dimension.y;
				this->w = dimension.w;
				this->h = dimension.h;
				this->size = dimension.size;
				this->d = dimension.d;
				this->id = dimension.id;
				this->direction = dimension.direction;
			}

			void _dump() {
				std::cout << "\n" << x << ", " << y << " (" << w << " / " << h << ")";
			}
		};
		template <typename T>
		struct HOLDER {
			HOLDER(std::initializer_list<T> ts) {
				this->ts = ts;
			};
			HOLDER() {};
			std::initializer_list<T> ts;

			T operator[](DINT position) {
				return *(this->ts.begin() + position);
			}
			DINT _size() {
				return (DINT)this->ts.size();
			}
		};
		template <typename ITEM>
		struct IMAP {
			struct PACKAGE {
				PACKAGE() {
					this->z = 0;
					this->exist = false;
					this->item = {};
				};

				PACKAGE(ITEM item, bool exist, double z) {
					this->item = item;
					this->exist = exist;
					this->z = z;
				}

				ITEM item;
				bool exist;
				double z;

				/*
				void operator = (PACKAGE p) {
					this->item = p.item;
					this->exist = p.exist;
					this->z = p.z;
				}
				*/

				void _dump() {
					std::cout << "\nPACKAGE: " << this->exist << " (" << this->z << ")";
				}
			};

			IMAP(DINT block = 8) {
				this->block = block;
				this->size = 0;
				this->length = 0;
				this->last = 0;
				this->counter = 0;
				this->package = nullptr;
				this->debug = false;
				this->z = false;
			};
			IMAP(std::initializer_list<ITEM> list) {
				this->block = 8;
				this->size = 0;
				this->length = 0;
				this->last = 0;
				this->counter = 0;
				this->package = nullptr;
				this->debug = false;
				this->z = false;
				drx::utils::HOLDER<ITEM> items = list;
				for (DINT i = 0; i < items._size(); i++) *this << items[i];
			}

			DINT block, size, length, last, counter;
			bool debug, z;
			PACKAGE* package;
			PACKAGE dummy;
			drx::utils::STRING name;

			void _resize(DINT size = 0) {
				DINT total = this->size;
				if (size > 0) {
					this->size += size;
				}
				else {
					this->size += this->block;
				}

				if (this->length > 0) {
					PACKAGE* package = new PACKAGE[total];
					DINT c = 0;
					for (DINT i = 0; i < total; i++) {
						if (this->package[i].exist) {
							package[c] = this->package[i];
							c++;
						}
					}
					delete[] this->package;
					this->package = new PACKAGE[this->size];
					for (DINT i = 0; i < total; i++) {
						if (package[i].exist) {
							this->package[i] = package[i];
						}
					}
					delete[] package;
				}
				else {
					this->package = new PACKAGE[this->size];
				}
			}

			void _swap(DINT a, DINT b) {
				PACKAGE pb = this->package[b];
				this->package[b] = this->package[a];
				this->package[a] = pb;
			}

			SLINT _sort(SINT dir = 1) {
				SLINT swaps = 0;
				for (DINT i = 0; i < this->size; i++) {
					for (DINT c = 0; c < this->size; c++) {
						if (i != c) {
							PACKAGE a = this->package[i];
							PACKAGE b = this->package[c];
							if (a.exist && b.exist) {
								switch (dir) {
								default:
									break;
								case -1:
									if (a.z < b.z) {
										this->_swap(i, c);
										swaps++;
									}
									break;
								case 1:
									if (a.z > b.z) {
										this->_swap(i, c);
										swaps++;
									}
									break;
								}
							}
						}
					}
				}


				return swaps;
			}

			void _s(DINT dir = 1) {

				SINT minc = 0, maxc = this->size - 1, min, max;
				min = (SINT)INFINITY;
				max = (SINT)-INFINITY;
				for (DINT i = 0; i < this->size; i++) {

				}

			}

			void operator << (ITEM item) {
				DICE dice;
				if (this->debug) std::cout << "\n> " << this->size << ", " << this->length;
				if (this->size == 0 || this->length == this->size) this->_resize();
				this->last = (this->last == this->size - 1) ? (0) : (this->last);
				for (DINT i = 0; i < this->size; i++) {
					if (!this->package[i].exist) {
						double z;
						if (this->z) {
							bool found;
							do {
								found = false;
								z = dice._droll(0, this->size);
								for (DINT c = 0; c < this->size; c++) {
									if (this->package[c].z == z) found = true;
								}
							} while (found);
						}
						else {
							z = (double)this->counter;
						}
						this->package[i] = { item, true, z };
						this->last = i;
						this->length++;
						this->counter++;
						break;
					}
				}
			}

			void operator >> (DINT pos) {
				if (pos >= 0 && pos < this->size) {
					this->last = (pos == this->size - 2) ? (0) : (pos);
					this->package[pos].exist = false;
					//this->package[pos].item = *item;
					this->length--;
				}
			}

			ITEM& operator [] (DINT pos) {
				if (pos >= 0 && pos < this->size) return this->package[pos].item;
				return this->dummy.item;
			}

			void _dump() {
				std::cout << "\nDUMPING " << this->length << " / " << this->size;
				for (DINT i = 0; i < this->size; i++) {
					//if (this->package[i].exist) {
					this->package[i]._dump();
					//}
				}
			}

			void _close() {
				if (this->size > 0) {
					delete[] this->package;
					this->size = 0;
					this->length = 0;
					this->last = 0;
					this->counter = 0;
					this->z = false;
				}
			}

			void _shift(drx::enums::DIRECTION dir) {
				switch (dir) {
				case drx::enums::DIRECTION::LEFT:
					for (DINT i = 0; i < this->length - 1; i++) {
						this->_swap(i, i + 1);
					}
					break;
				case drx::enums::DIRECTION::RIGHT:

					for (DINT i = this->length - 1; i > 0; i--) {
						this->_swap(i, i - 1);
					}
					break;
				}

			}

		};
		SLINT Tnth(DINT a, DINT th) {
			SLINT value = a;
			for (; th > 0; value *= 10) th--;
			return value;
		}
		struct INTC {
			INTC(SLINT large, SLINT rounded = 10) {
				DINT roundee = 0;
				this->rounder = rounded;
				if (rounded == -1) {
					for (SLINT i = large; i >= 10; i /= 10) {
						roundee++;
					}
					rounded = drx::utils::Tnth(1, roundee);
				}
				else {
					for (SLINT i = rounded; i > 1; i /= 10) roundee++;
				}
				if (large > 0) {
					for (; large > 0;) {
						for (DINT i = 0; i <= roundee; i++) {
							this->number << 0;
							for (; large > rounded - 1;) {
								large -= rounded;
								this->number[i] += 1;
							}
							rounded /= 10;
						}
					}
				}
				else {
					if (large < 0) {

					}
					else {
						this->number << 0;
					}
				}
				this->_number();
				this->large = 0;
				this->rounder = 0;
			};
			INTC() {
				this->large = 0;
				this->rounder = 0;
			};
			drx::utils::IMAP <SLINT> number;
			SLINT rounder;
			SLINT large;
			operator SLINT() const {
				return this->large;
			}
			operator drx::utils::INTC() const {
				return this->large;
			}
			void operator += (SLINT a) {
				*this = { this->_number() + a, -1 };

			}
			void operator -= (SLINT a) {
				SLINT n = this->_number() - a;
				if (n > 0) {
					*this = { n, -1 };
				}
				else {
					*this = { 0, 1 };
				}
			}
			void operator ++ (DINT a) {

				*this = { this->_number() + 1, -1 };
			}
			void operator -- (DINT b) {
				SLINT n = this->_number() - 1;
				if (n >= 0) {
					*this = { n, -1 };
				}
				else {
					*this = { 0, 1 };
				}

			}
			SLINT operator *= (SLINT a) {
				SLINT n = this->_number();
				*this = { n *= a, -1 };
				return *this;
			}
			SLINT operator /= (SLINT a) {
				SLINT n = this->_number();
				*this = { n /= a, -1 };
				return *this;
			}

			void operator << (SLINT a) {
				this->number << a;
			}

			SLINT _number() {
				SLINT n = 0;
				SLINT roundee = drx::utils::Tnth(1, this->number.length - 1);
				for (DINT i = 0; i < this->number.length; i++) {
					n += roundee * this->number[i];
					roundee /= 10;
				}
				this->large = n;
				return n;
			}

			SLINT _out() {
				SLINT n = 0;
				DINT p = this->number.length - 1;
				for (DINT i = 0; i < this->number.length; i++) {
					n += (SLINT)pow(p, this->number[i]);
					p--;
				}
				this->large = n;
				return n;
			}

			double _double() {
				//this->_dump(true);
				double r = 0.0;
				SLINT roundee = drx::utils::Tnth(1, this->number.length - 1);
				SLINT rd = roundee;
				bool nmet = false;
				for (DINT i = 0; i < this->number.length; i++) {
					r += (double)roundee * this->number[i];
					roundee /= 10;
				}
				r /= rd;
				return r;
			}

			SLINT _rnumber() {
				SLINT n = 0;
				return n;
			}

			void _dump(bool blocks = false) {
				if (!blocks) {
					std::cout << "\n>";
					for (DINT i = 0; i < this->number.length; i++) {
						std::cout << this->number[i];
					}
				}
				else {
					for (DINT i = 0; i < this->number.length; i++) {
						std::cout << "\n> number[" << i << "] = " << this->number[i];
					}
				}

			}

			void _close() {
				this->number._close();
				this->rounder = 0;
				this->large = 0;
			}

		};
		struct TIME {
			TIME() {
				this->year = 0;
				this->month = 0;
				this->day = 0;
				this->hour = 0;
				this->minute = 0;
				this->second = 0;
				this->millisecond = 0;
				this->microsecond = 0;
				this->nanosecond = 0;
				this->success = 0;
				this->precision = 0;
				this->count = 0;
				this->system = {};
				this->difference = 0;
				this->elapsed = 0;
				this->lh = 0;
				this->lm = 0;
				this->ls = 0;
				this->lms = 0;
			};
			~TIME() {
				this->year = 0;
				this->month = 0;
				this->day = 0;
				this->hour = 0;
				this->minute = 0;
				this->second = 0;
				this->millisecond = 0;
				this->microsecond = 0;
				this->nanosecond = 0;
				this->count = 0;
				this->elapsed = 0;
				this->difference = 0;
			};
			DINT year, month, day, hour, minute, second, success, precision, elapsed, difference;
			DINT millisecond, microsecond, nanosecond;
			LINT count;
			SYSTEMTIME system;
			DINT lh, lm, ls, lms;

			void _clock(DINT type = 1) {
				this->_reset();
				switch (type) {
				default:
					GetLocalTime(&this->system);
					this->month = this->system.wMonth;
					this->day = this->system.wDay;
					this->hour = this->system.wHour;
					this->minute = this->system.wMinute;
					this->second = this->system.wSecond;
					this->millisecond = this->system.wMilliseconds;
					this->microsecond = 0;
					this->nanosecond = 0;
					break;
				case 1:
					//std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
					std::chrono::steady_clock::time_point tp = std::chrono::high_resolution_clock::now();
					auto duration = tp.time_since_epoch();
					typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<2>>::type> Days;

					Days days = std::chrono::duration_cast<Days>(duration);
					this->day = (DINT)days.count();
					duration -= days;

					auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
					this->hour = (DINT)hours.count(); // +13;
					duration -= hours;

					auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
					duration -= minutes;
					this->minute = (DINT)minutes.count();

					auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
					duration -= seconds;
					this->second = (DINT)seconds.count();

					auto mss = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
					duration -= mss;
					this->millisecond = (DINT)mss.count();

					auto uss = std::chrono::duration_cast<std::chrono::microseconds>(duration);
					duration -= uss;
					this->microsecond = (DINT)uss.count();

					auto nss = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
					this->nanosecond = (DINT)nss.count();
					break;
				}
				DINT view = 0;
				if (view) {
					std::cout << "\nYear: " << this->year;
					std::cout << "\nMonth: " << this->month;
					std::cout << "\nDay: " << this->day;
					std::cout << "\nHour: " << this->hour;
					std::cout << "\nMinute: " << this->minute;
					std::cout << "\nSecond: " << this->second;
					std::cout << "\nMs: " << this->millisecond;
					std::cout << "\nMcrs: " << this->microsecond;
					std::cout << "\nNs: " << this->nanosecond;
				}
			}

			DINT _duration(TIME time, DINT ms = 0, DINT type = 0) {
				this->count = 0;
				this->_clock(type);
				this->hour = (this->hour > time.hour) ? ((this->hour == 0) ? (11) : (0)) : (this->hour);
				//this->count += (LINT)this->year * 12 * 30 * 24 * 60 * 60 * 1000;
				//this->count += (LINT)this->month * 30 * 24 * 60 * 60 * 1000;
				//this->count += (LINT)this->day * 24 * 60 * 60 * 1000;
				//this->count += (LINT)this->hour * 60 * 60 * 1000;
				this->count += (LINT)this->minute * 60 * 1000;
				this->count += (LINT)this->second * 1000;
				this->count += (LINT)this->millisecond;
				time.hour = (time.hour > this->hour) ? ((time.hour == 0) ? (11) : (0)) : (time.hour);
				//time.count += (LINT)time.year * 12 * 30 * 24 * 60 * 60 * 1000;
				//time.count += (LINT)time.month * 30 * 24 * 60 * 60 * 1000;
				//time.count += (LINT)time.day * 24 * 60 * 60 * 1000;
				//time.count += (LINT)time.hour * 60 * 60 * 1000;
				time.count += (LINT)time.minute * 60 * 1000;
				time.count += (LINT)time.second * 1000;
				time.count += (LINT)time.millisecond;
				switch (ms) {
				default:
					this->elapsed = (DINT)((this->count - time.count) / 1000);
					break;
				case 1:
					this->elapsed = (DINT)(this->count - time.count);
					break;
				}
				return this->elapsed;
			}
			void _increment(TIME time, bool reset = false) {
				if (reset) {
					this->ls = 0;
					this->lm = 0;
					this->lh = 0;
					this->lms = 0;
				}
				this->_clock(0);
				this->count += (LINT)this->hour * 60 * 60 * 1000;
				this->count += (LINT)this->minute * 60 * 1000;
				this->count += (LINT)this->second * 1000;
				this->count += (LINT)this->millisecond;
				time.count += (LINT)time.hour * 60 * 60 * 1000;
				time.count += (LINT)time.minute * 60 * 1000;
				time.count += (LINT)time.second * 1000;
				time.count += (LINT)time.millisecond;
				this->lms += (this->count > time.count) ? ((DINT)(this->count - time.count)) : ((DINT)(time.count - this->count));
				for (; this->lms > 999; this->lms -= 1000) {
					this->ls += 1;
				}
				for (; this->ls > 59; this->ls -= 60) {
					this->lm += 1;
				}
				for (; this->lm > 59; this->lm -= 60) {
					this->lh += 1;
				}
				this->count = 0;

			}
			void _reset() {
				this->count = 0;
				this->difference = 0;
				this->year = 0;
				this->month = 0;
				this->day = 0;
				this->hour = 0;
				this->minute = 0;
				this->second = 0;
				this->millisecond = 0;
				this->microsecond = 0;
				this->nanosecond = 0;
				/*
				this->count = 0;
				this->lh = 0;
				this->lms = 0;
				this->lm = 0;
				this->ls = 0;
				*/
			}

			void _difference(TIME time) {

			}

			DINT _since(DINT ms = 0) {
				DINT count = 0;
				count += this->lh * 60 * 60 * ((ms) ? (1000) : (1));
				count += this->lm * 60 * ((ms) ? (1000) : (1));
				count += this->ls * ((ms) ? (1000) : (1));
				count += (ms) ? (this->lms) : (0);
				return count;
			}

			drx::utils::STRING _took(bool out = true, bool ms = false) {
				/*
				STRING in;
				if (this->lh > 0) {
					in += WRITER::_itc(this->lh);
					in += "h ";
				}
				if (this->lm > 0) {
					in += WRITER::_itc(this->lm);
					in += "m ";
				}
				if (this->ls > 0) {
					in += WRITER::_itc(this->ls);
					in += "s ";
				}
				in += WRITER::_itc(this->lms);
				in += "ns ";
				*/
				drx::utils::STRING t;
				if (this->lh > 0) {
					//t = WRITER::_itc(this->lh);
					t += this->lh;
					t += "h ";
					if (out) std::cout << this->lh << "h ";
				}
				if (this->lm > 0) {
					//t += WRITER::_itc(this->lm);
					t += this->lm;
					t += "m ";
					if (out) std::cout << this->lm << "m ";
				}
				if (this->ls > 0) {
					//t += WRITER::_itc(this->ls);
					t += this->ls;
					t += "s ";
					if (out) std::cout << this->ls << "s ";
				}

				//t += WRITER::_itc(this->lms);
				if (ms) {
					t += this->lms;
					t += "ms ";
				}
				if (out) std::cout << this->lms << "ms ";
				return t;

			}
		};
		struct DIRECTORY {
			DIRECTORY(const char name[]) {
				std::cout << "\nDIRECTORY: " << name;
				if (std::filesystem::exists(std::filesystem::path({ name }))) {
					this->path._write(name);
					for (const auto& file : std::filesystem::directory_iterator(this->path.text)) {
						STRING f = file.path().string().c_str();
						for (; f._strip('.', '\\');) {
							//std::filesystem::current_path(std::filesystem::current_path().parent_path());
						}
						f._strip('s', '\\');
						//if (!f._match(".drx")) f._append(".drx");
						//f._dump();
						this->file << f;
					}
				}
			}
			DIRECTORY() {};
			drx::utils::STRING path;
			drx::utils::IMAP<drx::utils::STRING> file;


			void _dump() {
				std::cout << "\n Dumping /" << this->path.text;
				for (DINT i = 0; i < this->file.length; i++) {
					std::cout << "\n>" << this->file[i].text;
				}

			}

			SINT _search(const char file[]) {
				for (DINT f = 0; f < this->file.length; f++) {
					if (this->file.package[f].exist) {
						if (this->file[f]._match(file)) {
							return f;
						}
					}
				}
				return -1;
			}
		};
		struct GROUP {
			GROUP(DINT a, drx::utils::IMAP<DINT> g) {
				this->a = a;
				for (DINT b = 0; b < g.length; b++) {
					this->g << g[b];
				}
			};
			GROUP() {
				this->a = 0;
			};
			DINT a;
			drx::utils::IMAP<DINT> g;


			void _dump() {
				std::cout << "\n>" << this->a << " (";
				for (DINT i = 0; i < this->g.length; i++) {
					std::cout << this->g[i] << ", ";
				}
				std::cout << ")";
			}
		};

		struct PAIR {
			PAIR(DINT a, DINT b) {
				this->a = a;
				this->b = b;
			};
			PAIR() {
				this->a = 0;
				this->b = 0;
			};

			DINT a, b;
		};
		struct FACE {
			drx::utils::IMAP<DINT> i;
		};
		struct DUAL {
			DUAL() {
				this->i = 0;
				this->d = 0.0;
			};

			DINT i;
			double d;
		};
		struct COUNTER {
			COUNTER(DINT total = 0) {
				this->value = 0;
				this->current = total;
				this->total = total;
				this->count = 0;
			};
			~COUNTER() {
				this->value = 0;
				this->current = 0;
				this->total = 0;
				this->count = 0;
			};
			SINT value, current, total, count;

			operator SINT() const {
				return this->current;
			}
			void operator =(SINT value) {
				this->current = value;
			}
			void operator ++(SINT value) {
				this->current += value;
			}
			void _add(DINT value = 0) {
				this->current += value;
				this->total += value;
				this->count += value;
			}

			DINT _difference(COUNTER counter) {
				return this->current - counter.current;
			}

			void _reset() {
				this->current = 0;
				this->total = 0;
				this->value = 0;
			}

			DINT _full() {
				return (this->current >= this->total) ? (1) : (0);
			}

			void _set(DINT n) {
				this->current = n;
				this->total = n;
				this->count = n;
			}
		};
		template <typename TYPE>
		struct STAT {
			STAT(const char name[]) {
				this->name = name;
				this->current = 0;
				this->total = 0;
			};
			STAT() {
				this->current = 0;
				this->total = 0;
			};
			drx::utils::STRING name;
			TYPE current, total;

			void _add(TYPE value) {
				this->total += value;
				this->current += value;
			}

			void _set(TYPE value) {
				this->current = value;
				this->total = value;
			}
		};
		struct POS {
			POS(SINT x = 0, SINT y = 0) {
				this->x = x;
				this->y = y;
				this->w = 0;
				this->h = 0;
				this->blocked = 0;
				this->checked = 0;
				this->total = 0;
			}
			POS(SINT x, SINT y, SINT w, SINT h) {

				this->x = x;
				this->y = y;
				this->w = w;
				this->h = h;
				this->blocked = 0;
				this->checked = 0;
				this->total = 0;
			}
			SINT x, y, w, h, checked, total;
			bool blocked;
			void _dump() {
				std::cout << "\nPOS: " << this->x << ", " << this->y;
			}

			DINT operator == (POS b) {
				if (b.x == this->x && b.y == this->y) return 1;
				return 0;
			}

			DINT operator != (POS b) {
				if (b.x != this->x || b.y != this->y) return 1;
				return 0;
			}

			DINT _direction(POS next) {
				DINT dir = 5;
				if (this->x < next.x && this->y == next.y) dir = 6;
				if (this->x > next.x && this->y == next.y) dir = 4;
				if (this->x == next.x && this->y < next.y) dir = 2;
				if (this->x == next.x && this->y > next.y) dir = 8;
				//std::cout << "\n Dir: " << dir;
				return dir;
			}

		};
		SINT Clamp(SINT min, SINT value, SINT max) {
			if (value < min) value = min;
			if (value > max) value = max;
			return value;
		}
		
		/*
		DINT Clamp(DINT min, DINT value, DINT max) {
			if (value < min) value = min;
			if (value > max) value = max;
			return value;
		}
		*/

		double Clamp(double min, double& value, double max) {
			if (value < min) value = min;
			if (value > max) value = max;
			return value;
		}
		char Ntc(DINT num) {
			if (num == 0) return '0';
			if (num == 1) return '1';
			if (num == 2) return '2';
			if (num == 3) return '3';
			if (num == 4) return '4';
			if (num == 5) return '5';
			if (num == 6) return '6';
			if (num == 7) return '7';
			if (num == 8) return '8';
			if (num == 9) return '9';
			return '?';
		}
		DINT Ctn(char num) {
			switch (num) {
			default:
			case '0':
				return 0;
			case '1':
				return 1;
			case '2':
				return 2;
			case '3':
				return 3;
			case '4':
				return 4;
			case '5':
				return 5;
			case '6':
				return 6;
			case '7':
				return 7;
			case '8':
				return 8;
			case '9':
				return 9;
			}
		}
		SINT Isnum(const char character) {
			if (character == '0') return 0;
			if (character == '1') return 1;
			if (character == '2') return 2;
			if (character == '3') return 3;
			if (character == '4') return 4;
			if (character == '5') return 5;
			if (character == '6') return 6;
			if (character == '7') return 7;
			if (character == '8') return 8;
			if (character == '9') return 9;
			return -1;

		}
		double Iatdz(drx::utils::IMAP<DINT> nums) {
			double value = 0.0;
			//nums._reverse();  // TO DO
			for (DINT i = 0; i < nums.length; i++) {
				value += (double)nums[i];
				value /= 10.0;
			}
			nums._close();
			return value;
		}
		DINT Iati(drx::utils::IMAP<DINT> nums) {
			//nums._reverse(); // TO DO
			DINT value = 0;
			for (DINT i = 0; i < nums.length; i++) {
				DINT v = nums[i];
				DINT c = i;
				for (; c > 0; c--) {
					v *= 10;
				}
				value += v;
			}
			nums._close();
			return value;
		}
		bool OnLineExact(drx::utils::SPOT a, drx::utils::SPOT b, drx::utils::SPOT p) {

			if (a.x > b.x) {
				drx::utils::SPOT c = { a.x, a.y };
				a = { b.x, b.y };
				b = { c.x, c.y };
			}
			//if (a.x == p.x || a.y == p.y && b.x == p.x || b.y == p.y) return true;
			//if (a.x == p.x) return b.x == p.x;
			//if (a.y == p.y) return b.y == p.y;
			return (a.x - p.x) * (a.y - p.y) == (p.x - b.x) * (p.y - b.y);

		}
		void MakePoint(drx::utils::IMAP < drx::utils::SPOT > spots, drx::utils::SSPOT* p) {
			for (DINT i = 0; i < spots.length; i++) {
				drx::utils::SPOT s = spots[i];
				if (s.x < p->x) p->x = s.x;
				if (s.x > p->w) p->w = s.x;
				if (s.y < p->y) p->y = s.y;
				if (s.y > p->h) p->h = s.y;
				if (s.z < p->z) p->z = s.z;
				if (s.z > p->d) p->d = s.z;
			}
		}
		double Gap(double value, double range = 2.0) {
			DICE dice;
			double a = value - range, b = value + range;
			if (a < b) {
				return dice._droll(a, b);
			}
			else {
				return dice._droll(b, a);
			}
		}
		bool Collision(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2) {
			if (
				x1 <= x2 + w2 &&
				x1 + w1 >= x2 &&
				y1 <= y2 + h2 &&
				y1 + h1 >= y2
				) return true;
			return false;

		}
		bool Oob(SINT a, SINT b, SINT x, SINT y, SINT w, SINT h) {
			if (a <= x) return true;
			if (b <= y) return true;
			if (a >= w) return true;
			if (b >= h) return true;
			return false;
		}
		struct EXA : SPOT {
			EXA(double x, double y, double z, std::initializer_list<DINT> ps = {}) {
				drx::utils::HOLDER pairs = ps;
				for (DINT p = 0; p < pairs._size(); p++) this->pair << pairs[p];
				this->x = x;
				this->y = y;
				this->z = z;
			};
			EXA(drx::utils::SPOT spot, drx::utils::IMAP<DINT> pairs, drx::utils::IMAP<DINT> groups) {
				this->x = spot.x;
				this->y = spot.y;
				this->z = spot.z;
				for (DINT p = 0; p < pairs.length; p++) this->pair << pairs[p];
				for (DINT g = 0; g < groups.length; g++) this->group << groups[g];
			};
			EXA() {};
			drx::utils::IMAP<DINT> pair;
			drx::utils::IMAP<DINT> group;
			drx::utils::SPOT _spot() {
				return { this->x, this->y, this->z };
			}
		};
		struct DATA {
			DATA() {
				this->type = -1;
			};

			drx::utils::STRING file;
			std::fstream stream;
			SINT type;
			drx::utils::IMAP<drx::utils::EXA> exas;
			bool _read(const char file[], DINT type = 0, bool debug = false) {

				bool loaded = false;
				switch (type) {
				default:
					this->file._write(ENGINE_MAP_DIRECTORY_SPRITE);
					this->file._app('\\');
					this->file._append(file);
					if (!this->file._match(".drx")) this->file._append(".drx");
					break;
				case 1:
					this->file._write(ENGINE_MAP_DIRECTORY_POLYGON);
					this->file._app('\\');
					this->file._append(file);
					if (!this->file._match(".pdrx")) this->file._append(".pdrx");
					break;
				}
				this->stream.open(this->file.text);
				bool open = this->stream.is_open();
				if (open) {
					char cursor;
					drx::utils::IMAP<char> content;
					//STRING cont;
					while (this->stream.good()) {
						this->stream >> cursor;
						//cont._app(cursor);
						content << cursor;
					}
					//std::cout << "\n" << cont.text;
					if (debug) std::cout << "\nRead a content of " << content.length << " characters. (" << this->file.text << ")";

					for (DINT c = 0; c < content.length; c++) {
						cursor = content[c];
						if (cursor == '{') {
							drx::utils::EXA exa;
							drx::utils::IMAP<DINT> x;
							bool neg = false;
							c++;
							do {
								if (content[c] == '-') neg = true; else x << drx::utils::Isnum(content[c]);
								c++;
							} while (content[c] != '.');
							exa.x = (double)drx::utils::Iati(x);
							if (neg) exa.x *= -1.0;
							neg = false;


							drx::utils::IMAP<DINT> dx;

							c++;
							do {
								dx << drx::utils::Isnum(content[c]);
								c++;
							} while (content[c] != ',');
							exa.x += drx::utils::Iatdz(dx);

							drx::utils::IMAP<DINT> y;
							c++;
							do {
								if (content[c] == '-') neg = true; else y << drx::utils::Isnum(content[c]);
								c++;
							} while (content[c] != '.');
							exa.y = (double)drx::utils::Iati(y);
							if (neg) exa.y *= -1.0;
							neg = false;

							drx::utils::IMAP<DINT> dy;
							c++;
							do {
								dy << drx::utils::Isnum(content[c]);
								c++;
							} while (content[c] != ',');
							exa.y += drx::utils::Iatdz(dy);


							drx::utils::IMAP<DINT> z;
							c++;
							do {
								if (content[c] == '-') neg = true; else z << drx::utils::Isnum(content[c]);
								c++;
							} while (content[c] != '.');
							exa.z = (double)drx::utils::Iati(z);
							if (neg) exa.z *= -1.0;

							drx::utils::IMAP<DINT> dz;

							c++;
							do {
								dz << drx::utils::Isnum(content[c]);
								c++;
							} while (content[c] != ',' && content[c] != '}');
							exa.z += drx::utils::Iatdz(dz);



							if (content[c] == ',') {
								//std::cout << "\nPairing.";
								c += 2;
								do {
									drx::utils::IMAP<DINT> prs;
									//std::cout << "\n";
									if (content[c] != '}') {
										do {
											//std::cout << content[c];
											SINT n = drx::utils::Isnum(content[c]);
											if (n != -1) prs << (DINT)n;
											c++;
										} while (content[c] != ',' && content[c] != '}');
										exa.pair << drx::utils::Iati(prs);
									}
									if (content[c] != '}') c++;
								} while (content[c] != ',' && content[c] != '}');
								if (content[c + 1] == ',') {
									c++;
									//std::cout << "\nGrouping.";
									do {
										drx::utils::IMAP<DINT> grps;
										do {
											SINT n = drx::utils::Isnum(content[c]);
											if (n != -1) grps << (DINT)n;
											c++;
										} while (content[c] != ',' && content[c] != '}');
										exa.group << drx::utils::Iati(grps);
									} while (content[c] != '}');
								}
								else {
									//c++
								}
							}
							else {
								//c++;
							}

							//std::cout << "\nSPOT: " << exa.x << ", " << exa.y << ", " << exa.z << " - ";
							//for (DINT p = 0; p < exa.pair.length; p++) std::cout << exa.pair[p] << " ";
							this->exas << exa;
						}
						//	break;
					}

					content._close();
					this->stream.close();
					loaded = true;
				}
				return loaded;
			}



		};

	}
	namespace math {

		double Remainder(double d) {
			for (; d >= 1.0; d -= 1.0);
			return d;
		}
		double Pi() {
			return 3.1415926535;
		}
		double Rad(double degree = 90.0) {
			return degree * (drx::math::Pi() / 180.0);
		}
		double Deg(double rad = 1.0) {
			return rad * (180.0 / drx::math::Pi());
		}
		SLINT Abs(SLINT value = -1) {
			if (value < 0) {
				value = (-1) * value;
			}
			return value;
		}
		double Abs(double value = -1.0) {
			if (value < 0) {
				value = (-1.0) * value;
			}
			return value;
		}
		drx::utils::SPOT Transform2D(drx::utils::SPOT p, drx::utils::SPOT angle, bool rad = false) {
			for (; angle.x >= 360.0; angle.x -= 360.0);
			drx::utils::SPOT n;
			double c, s;
			if (rad) {
				c = cos(drx::math::Rad(angle.x));
				s = sin(drx::math::Rad(angle.x));
			}
			else {
				c = cos(angle.x);
				s = sin(angle.x);
			}
			n.x = p.x * c - p.y * s;
			n.y = p.x * s + p.y * c;
			return n;
		}
		drx::utils::SPOT Transform3D(drx::utils::SPOT p, drx::utils::SPOT angle, bool rad = false) {
			drx::utils::SPOT c, s;
			if (rad) {
				c = { cos(drx::math::Rad(angle.x)), cos(drx::math::Rad(angle.y)), cos(drx::math::Rad(angle.z)) };
				s = { sin(drx::math::Rad(angle.x)), sin(drx::math::Rad(angle.y)), sin(drx::math::Rad(angle.z)) };
			}
			else {
				c = { cos(angle.x), cos(angle.y), cos(angle.z) };
				s = { sin(angle.x), sin(angle.y), sin(angle.z) };
			}
			drx::utils::SPOT n; // = MATH::_transform2D(p, angle);

			n.y = p.y * c.x - p.z * s.x;
			n.z = p.y * s.x + p.z * c.x;

			n.x = p.x * c.y + n.z * s.y;
			n.z = p.x * s.y - n.z * c.y;

			//n.y = n.y * c.z + p.x * s.z;
			//n.x = p.y * s.z - n.x * c.z;


			return n;

		}
		double Distance(drx::utils::SPOT a, drx::utils::SPOT b, bool radial = false) {
			if (radial) {
				double x = b.x - a.x;
				double y = b.y - a.y;
				double z = x * x + y * y;
				return sqrt(z);
			}
			else {
				double x = b.x - a.x;
				double y = b.y - a.y;
				return drx::math::Abs(x + y);
			}

		}
		double PosAngle(drx::utils::SPOT a, drx::utils::SPOT b, double degree = 0.0) {
			double radius = drx::math::Distance(a, b);
			//SPOT c = { a.x + cos(MATH::_rad(degree + 90.0)) * radius, a.y + sin(MATH::_rad(degree + 90.0)) * radius };
			double rad = atan2(b.y - a.y, b.x - a.x);
			double deg = drx::math::Deg(rad);
			/*
			//double rad = atan(slope);
			double deg = MATH::_dabs(MATH::_deg(rad));
			double calc = 0.0;
			if (a.x >= c.x && a.y <= c.y) {
				// 0 - 90
				std::cout << "\n 0 ";
				calc = deg - degree - 90.0;
			}
			else if (a.x >= c.x && a.y >= c.y) {
				// 90 - 180
				std::cout << "\n 1 ";
				calc = deg - degree + 270.0;
			}
			else if (a.x <= c.x && a.y >= c.y) {
				// -180 - -90
				std::cout << "\n 2 ";
				calc = deg - degree + 180.0;
			}
			else if (a.x <= c.x && a.y <= c.y) {
				// -90 - 0
				std::cout << "\n 3 ";
				calc = deg - degree + 90.0;
			}
			else {
				// err
			}


			std::cout << rad << ", " <<  deg << ", " << calc;
			*/

			return deg;
		}


	}
	namespace gfx {
		struct DYE {
			DYE(DINT color) {
				this->value = color;
			};
			DYE() {
				this->value = 0;
			};

			DINT value;


			operator DINT() const {
				return this->value;
			}

			void operator=(DINT value) {
				this->value = drx::utils::Clamp(0, value, 255);
			}

			DINT _darken(DINT amount = 0) {
				this->value = drx::utils::Clamp(0, this->value - amount, 255);
				return this->value;
			}

			DINT _lighten(DINT amount = 0) {
				this->value = drx::utils::Clamp(0, this->value + amount, 255);
				return this->value;
			}
			void operator+=(DINT value) {
				this->value = drx::utils::Clamp(0, this->value + value, 255);
			}
			void operator-=(DINT value) {
				this->value = drx::utils::Clamp(0, this->value - value, 255);
			}

		};
		struct COLOR {
			COLOR(DINT red = 255, DINT green = 255, DINT blue = 255, DINT exist = 0, DINT brk = 0, DINT alpha = 255) {
				red = drx::utils::Clamp(0, red, 255);
				green = drx::utils::Clamp(0, green, 255);
				blue = drx::utils::Clamp(0, blue, 255);
				this->red = red;
				this->green = green;
				this->blue = blue;
				this->alpha = alpha;
				if (exist == 1) this->exist = true;
				if (exist == 0) this->exist = false;
				if (brk == 1) this->brk = true;
				if (brk == 0) this->brk = false;
				this->_code();
			}
			DYE red, green, blue, alpha;
			bool exist, brk;
			unsigned long code;
			COLORREF ref;

			void operator = (drx::gfx::COLOR color) {
				this->red = color.red;
				this->green = color.green;
				this->blue = color.blue;
				this->alpha = color.alpha;
				this->exist = color.exist;
				this->brk = color.brk;
				this->_code();
			}

			void _code() {
				this->code = this->red << 16;
				this->code += this->green << 8;
				this->code += this->blue;
				this->ref = RGB(this->red, this->green, this->blue);
			}
			void _set(COLOR color) {
				color.red = drx::utils::Clamp(0, color.red, 255);
				color.green = drx::utils::Clamp(0, color.green, 255);
				color.blue = drx::utils::Clamp(0, color.blue, 255);
				this->red = color.red;
				this->green = color.green;
				this->blue = color.blue;
				this->exist = color.exist;
				this->brk = color.brk;
				this->_code();
			}
			void _dye(DINT r, DINT g, DINT b) {
				r = drx::utils::Clamp(0, r, 255);
				g = drx::utils::Clamp(0, g, 255);
				b = drx::utils::Clamp(0, b, 255);
				this->red = r;
				this->green = g;
				this->blue = b;
				this->exist = true;
				this->_code();
#include "../assets/engine.literary.h"
			}
			drx::gfx::COLOR _darken(DINT amount) {

				drx::gfx::COLOR c = { this->red._darken(amount), this->green._darken(amount), this->blue._darken(amount), 1 };
				return c;
			}
			drx::gfx::COLOR _lighten(DINT amount) {
				drx::gfx::COLOR c = { this->red._lighten(amount), this->green._lighten(amount), this->blue._lighten(amount), 1 };
				return c;
			}
			bool operator == (COLOR c) {
				if (this->red != c.red) return false;
				if (this->green != c.green) return false;
				if (this->blue != c.blue) return false;
				if (this->exist != c.exist) return false;
				return true;
			}
			/*
			bool operator == (std::initializer_list<DINT> rgbe) {
				drx::utils::HOLDER<DINT> v = rgbe;
				DINT r = drx::utils::Clamp(0, v[0], 255);
				DINT g = drx::utils::Clamp(0, v[1], 255);
				DINT b = drx::utils::Clamp(0, v[2], 255);
				bool e = (v[3] == 0) ? (false) : (true);
				if (this->red != r) return false;
				if (this->green != g) return false;
				if (this->blue != b) return false;
				if (this->exist != e) return false;
				return true;
			}
			*/
			bool operator != (COLOR c) {
				if (this->red == c.red && this->green == c.green && this->blue == c.blue && this->exist == c.exist) return false;
				return true;
			}

			void _dump() {
				std::cout << "\n>COLOR: " << this->red << ", " << this->green << ", " << this->blue << ", " << this->exist;
			}
		}; 
		struct STATE {
			STATE() {
				this->x = 0;
				this->y = 0;
				this->w = 0;
				this->h = 0;
				this->pixel = {};
				this->bi = {};
				this->init = false;
				this->handle = 0;
				this->memory = {};
			}
			SINT w, h, x, y;
			void* memory;
			BITMAPINFO bi;
			ULINT* pixel;
			HWND handle;
			bool init;

			void Alloc() {
				if (this->memory) VirtualFree(this->memory, 0, MEM_RELEASE);
				this->memory = VirtualAlloc(0, this->w * this->h * sizeof(unsigned int), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			}
			void Resize(DINT w, DINT h) {
				this->w = w;
				this->h = h;
				this->Alloc();
				this->bi.bmiHeader.biWidth = this->w;
				this->bi.bmiHeader.biHeight = this->h;

			}
			void Init(HWND handle, DINT w, DINT h) {
				this->x = 0;
				this->y = 0;
				this->w = w;
				this->h = h;
				if (handle != NULL) this->handle = handle;
				this->Alloc();
				this->bi.bmiHeader.biSize = sizeof(BITMAPINFO);
				this->bi.bmiHeader.biWidth = w;
				this->bi.bmiHeader.biHeight = h;
				this->bi.bmiHeader.biPlanes = 1;
				this->bi.bmiHeader.biBitCount = 32;
				this->bi.bmiHeader.biCompression = BI_RGB;
				this->init = true;
				std::cout << "\nCanvas created";
			}
			void Set(DINT x, DINT y, drx::gfx::COLOR color, DINT w = 1, DINT h = 1) {
				DINT fx = drx::utils::Clamp(0, x, this->w);
				DINT fy = drx::utils::Clamp(0, y, this->h);
				DINT tx = drx::utils::Clamp(0, x + w, this->w);
				DINT ty = drx::utils::Clamp(0, y + h, this->h);

				//color = { color.blue, color.green, color.red }; // Enable if color.ref used
				for (DINT y = fy; y < ty; y++) {
					this->pixel = (ULINT*)this->memory + (ULINT)(this->h * this->w - (y + 1) * this->w + fx);
					//this->pixel = (ULINT*)this->memory + (ULINT)(y * this->w + px);
					for (SINT x = fx; x < tx; x++) {
						*this->pixel++ = color.code; // color.ref = BGR, color.code = RGB;
					}
				}
			}
			void Draw() {
				HDC tool = GetDC(this->handle);
				StretchDIBits(tool, this->x, this->y, this->w, this->h, 0, 0, this->w, this->h, this->memory, &this->bi, DIB_RGB_COLORS, SRCCOPY);
				ReleaseDC(this->handle, tool);
			}
			void Clear(drx::gfx::COLOR color = ENGINE_BACKGROUND) {
				this->pixel = (ULINT*)this->memory;
				for (DINT y = 0; y < this->h; y++) {
					for (DINT x = 0; x < this->w; x++) {
						*this->pixel++ = color.ref;
					}
				}
			}
			void _blt(DINT x, DINT y, COLOR color, DINT w = 1, DINT h = 1) {
				HDC tool = GetDC(this->handle);
				HDC temp = CreateCompatibleDC(tool);
				HBITMAP bm = CreateCompatibleBitmap(tool, w, h);
				HANDLE old = SelectObject(temp, bm);
				RECT rect = { 0, 0, w, h};
				HBRUSH brush = CreateSolidBrush(color.ref);
				FillRect(temp, &rect, brush);

				BitBlt(tool, x, y, w, h, temp, 0, 0, SRCCOPY);
				ReleaseDC(this->handle, tool);
				DeleteDC(temp);
				DeleteObject(bm);
				DeleteObject(brush);
			}
		};
		drx::gfx::STATE canvas;
		drx::gfx::COLOR transparent = X;
		struct PIXEL {
			PIXEL(DINT x = 0, DINT y = 0, drx::gfx::COLOR c = {}) {
				this->x = x;
				this->y = y;
				this->color = c;
				this->apply = false;
			};
			drx::gfx::COLOR color;
			DINT x, y;
			bool apply;

			void _dump() {
				std::cout << "\nPIXEL: " << this->x << ", " << this->y << " (" << this->color.red << ", " << this->color.green << ", " << this->color.blue << ", " << this->color.exist << ")";
			}
		};
		void Set(DINT x, DINT y, drx::gfx::COLOR color = B, DINT w = 1, DINT h = 1) {
			DINT fx = drx::utils::Clamp(0, x, drx::gfx::canvas.w);
			DINT fy = drx::utils::Clamp(0, y, drx::gfx::canvas.h);
			DINT tx = drx::utils::Clamp(0, x + w, drx::gfx::canvas.w);
			DINT ty = drx::utils::Clamp(0, y + h, drx::gfx::canvas.h);
			if (drx::gfx::canvas.bi.bmiHeader.biHeight < 0) {
				for (DINT y = fy; y < ty; y++) {
					//gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(gfx::canvas.h * gfx::canvas.w - (y + 1) * gfx::canvas.w + fx);
					gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(y * gfx::canvas.w + x);
					for (SINT x = fx; x < tx; x++) {
						*gfx::canvas.pixel++ = color.code; // color.ref = BGR, color.code = RGB;
					}
				}
			}
			else {
				for (DINT y = fy; y < ty; y++) {
					gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(gfx::canvas.h * gfx::canvas.w - (y + 1) * gfx::canvas.w + fx);
					//gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(y * gfx::canvas.w + x);
					for (SINT x = fx; x < tx; x++) {
						*gfx::canvas.pixel++ = color.code; // color.ref = BGR, color.code = RGB;
					}
				}
			}

		}
		void Mix(DINT x, DINT y, drx::gfx::COLOR color, DINT w, DINT h) {
			DINT fx = drx::utils::Clamp(0, x, drx::gfx::canvas.w);
			DINT fy = drx::utils::Clamp(0, y, drx::gfx::canvas.h);
			DINT tx = drx::utils::Clamp(0, x + w, drx::gfx::canvas.w);
			DINT ty = drx::utils::Clamp(0, y + h, drx::gfx::canvas.h);
			if (drx::gfx::canvas.bi.bmiHeader.biHeight < 0) {
				for (DINT y = fy; y < ty; y++) {
					//gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(gfx::canvas.h * gfx::canvas.w - (y + 1) * gfx::canvas.w + fx);
					gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(y * gfx::canvas.w + x);
					for (SINT x = fx; x < tx; x++) {
						*gfx::canvas.pixel++ &= color.code; // color.ref = BGR, color.code = RGB;
					}
				}
			}
			else {
				for (DINT y = fy; y < ty; y++) {
					gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(gfx::canvas.h * gfx::canvas.w - (y + 1) * gfx::canvas.w + fx);
					//gfx::canvas.pixel = (ULINT*)gfx::canvas.memory + (ULINT)(y * gfx::canvas.w + x);
					for (SINT x = fx; x < tx; x++) {
						*gfx::canvas.pixel++ &= color.code; // color.ref = BGR, color.code = RGB;
					}
				}
			}
		}
		struct LITERAL {
			LITERAL(std::initializer_list<drx::gfx::COLOR> colors, DINT width = 3, DINT height = 5) {
				drx::utils::HOLDER<drx::gfx::COLOR> holder = colors;
				this->size.w = width;
				this->size.h = height;
				PIXEL pixel;
				DINT p = 0;
				//std::cout << "\n> Lettering colors. " << this->size.w << ", " << this->size.h;
				for (DINT x = 0; x < this->size.w; x++) {
					for (DINT y = 0; y < this->size.h; y++) {
						pixel = { x, y, holder[p] };
						//std::cout << "\n> " << pixel.x << ", " << pixel.y << " (" << pixel.color.r << ", " << pixel.color.g << ", " << pixel.color.b << ")";
						this->pixels << pixel;
						p++;
					}
				}
				//std::cout << " - " << this->pixels.length;
			};
			LITERAL() {
				this->size = { 0,0, 0,0 };
			};
			~LITERAL() {};

			drx::utils::IMAP<drx::gfx::PIXEL> pixels;
			drx::utils::DIMENSION size;

			drx::gfx::PIXEL _get(DINT x, DINT y) {
				for (DINT i = 0; i < this->pixels.size; i++) {
					if (this->pixels[i].x == x && this->pixels[i].y == y) return this->pixels[i];
				}
				drx::gfx::PIXEL p;
				return p;
			}
		};
		struct LIT {
			LIT(double identifier, int key, int numeric, const char lower, const char upper, const drx::gfx::LITERAL lr, const drx::gfx::LITERAL hr = {}, const drx::gfx::LITERAL hrlc = {}) {
				this->identifier = identifier;
				this->numeric = numeric;
				this->key = key;
				this->upper = upper;
				this->lower = lower;
				this->lr = lr;
				this->lr.pixels.name = "l";
				this->lr.pixels.name._app(lower);
				this->hr = hr;
				this->hr.pixels.name = "h";
				this->hr.pixels.name._app(upper);
				this->hrlc = hrlc;
				this->hrlc.pixels.name = "hlc";
				this->hrlc.pixels.name._app(lower);
				this->usage = 0;
				this->exist = true;
			};
			LIT() {
				this->identifier = 0.0;
				this->numeric = -1;
				this->key = -1;
				this->upper = '#';
				this->lower = '#';
				this->usage = 0;
				this->exist = false;
			}
			double identifier;
			SINT numeric, key;
			DINT usage;
			char upper, lower;
			drx::gfx::LITERAL lr, hr, hrlc;
			bool exist;
		};
		struct SPRITE {
			SPRITE() {
				this->loaded = false;
				this->scale = { 1.0, 1.0 };
			};
			drx::utils::IMAP <drx::utils::IMAP<drx::gfx::PIXEL>> pixels, draw;
			drx::utils::DIMENSION size, dim;
			drx::utils::STRING name;
			bool loaded;
			drx::utils::SPOT scale;
			HDC temp;

			void _close() {
				for (DINT i = 0; i < this->pixels.length; i++) {
					this->pixels.package[i].item._close();
				}
				this->pixels._close();
				this->draw._close();
				this->size = { 0, 0, 0, 0 };
				this->loaded = false;
				this->name = "";
			}

			void _position(DINT x, DINT y) {
				this->size.x = x;
				this->size.y = y;
			}

			void _position(double x, double y) {
				this->size.x = (DINT)x;
				this->size.y = (DINT)y;
			}

			void _hue(COLOR color, double r = 0.5, double g = 0.5, double b = 0.5, std::initializer_list<drx::gfx::COLOR> avoid = {}) {
				drx::utils::HOLDER<drx::gfx::COLOR> block = avoid;
				drx::utils::Clamp(0.0, r, 1.0);
				drx::utils::Clamp(0.0, g, 1.0);
				drx::utils::Clamp(0.0, b, 1.0);
				double rfill = r;
				double rretain = 1.0 - rfill;
				double gfill = g;
				double gretain = 1.0 - gfill;
				double bfill = b;
				double bretain = 1.0 - bfill;

				drx::gfx::COLOR trans = X;
				for (DINT x = 0; x < this->size.w; x++) {
					for (DINT y = 0; y < this->size.h; y++) {
						drx::gfx::COLOR clr = this->pixels[x][y].color;
						drx::gfx::COLOR* dclr = &this->draw[x][y].color;
						bool hue = true;
						for (DINT i = 0; i < block._size(); i++) {
							if (clr == block[i]) {
								hue = false;
								break;
							}
						}
						if (hue && *dclr != trans) dclr->_dye(color.red * rfill + clr.red * rretain, color.green * gfill + clr.green * gretain, color.blue * bfill + clr.blue * bretain);
					}
				}

			}

			drx::gfx::SPRITE _center(drx::gfx::SPRITE s, DINT w = 16, DINT h = 16, DINT sx = 0, DINT sy = 0, drx::enums::DIRECTION align = drx::enums::DIRECTION::CENTER) {
				drx::gfx::SPRITE r;
				r.size.w = w;
				r.size.h = h;
				for (DINT x = 0, ax = 0; x < r.size.w; x++) {
					drx::utils::IMAP<drx::gfx::PIXEL> apixs;
					for (DINT y = 0, ay = 0; y < r.size.h; y++) {
						drx::gfx::PIXEL p;
						switch (align) {
						default:
						case drx::enums::DIRECTION::CENTER:
							if (x >= sx && y >= sy && x < sx + s.size.w && y < sy + s.size.h) {
								p.color._set(s.pixels[ax][ay].color);
							}
							else {
								p.color._set(X);
							}
							break;
						case drx::enums::DIRECTION::UP:
							if (x >= sx && x < sx + s.size.w && y > 0) {
								p.color._set(s.pixels[ax][y - 1].color);
							}
							else {
								p.color._set(X);
							}
							break;
						}
						p.x = x;
						p.y = y;
						if (y >= sy) ay++;
						apixs << p;
					}
					if (x >= sx) ax++;
					r.pixels << apixs;
					r.draw << apixs;
				}
				return r;
			}

			drx::gfx::SPRITE _junction(drx::gfx::SPRITE s, drx::enums::MASH mash = drx::enums::MASH::HALF, drx::enums::DIRECTION align = drx::enums::DIRECTION::CENTER) {
				DINT w = 0, h = 0, px = 0, py = 0;
				DINT c = 0;
				if (s.size.w > this->size.w) {
					w = s.size.w;
					px = s.size.w / 2 - this->size.w / 2;
				}
				else {
					w = this->size.w;
					px = this->size.w / 2 - s.size.w / 2;
				}
				if (s.size.h > this->size.h) {
					h = s.size.h;
					py = s.size.h / 2 - this->size.h / 2;
				}
				else {
					h = this->size.h;
					py = this->size.h / 2 - s.size.h / 2;
				}

				w += 2;
				h += 2;
				drx::gfx::SPRITE a;
				a = this->_center(*this, w, h, w / 2 - this->size.w / 2, h / 2 - this->size.h / 2);

				drx::gfx::SPRITE b;
				b = this->_center(s, w, h, w / 2 - s.size.w / 2, h / 2 - s.size.h / 2, align);


				drx::gfx::SPRITE n;

				for (DINT x = 0; x < w; x++) {
					drx::utils::IMAP<drx::gfx::PIXEL> pixs;
					for (DINT y = 0; y < h; y++) {
						drx::gfx::PIXEL pa = a.pixels[x][y], pb = b.pixels[x][y];
						drx::gfx::PIXEL p;
						if (pa.color.exist && pb.color.exist) {
							DINT r = 0, g = 0, b = 0;
							switch (mash) {
							default:
							case drx::enums::MASH::HALF:
								r = pa.color.red / 2 + pb.color.red / 2;
								g = pa.color.green / 2 + pb.color.green / 2;
								b = pa.color.blue / 2 + pb.color.blue / 2;
								break;
							case drx::enums::MASH::MINUS:
								r = (pa.color.red > pb.color.red) ? (pa.color.red - pb.color.red) : (pb.color.red - pa.color.red);
								g = (pa.color.green > pb.color.green) ? (pa.color.green - pb.color.green) : (pb.color.green - pa.color.green);
								b = (pa.color.blue > pb.color.blue) ? (pa.color.blue - pb.color.blue) : (pb.color.blue - pa.color.blue);
								break;
							case drx::enums::MASH::PLUS:
								r = pa.color.red + pb.color.red;
								g = pa.color.green + pb.color.green;
								b = pa.color.blue + pb.color.blue;
								break;
							case drx::enums::MASH::DOMINANT:
								r = (pa.color.red > pb.color.red) ? (pa.color.red) : (pb.color.red);
								g = (pa.color.green > pb.color.green) ? (pa.color.green) : (pb.color.green);
								b = (pa.color.blue > pb.color.blue) ? (pa.color.blue) : (pb.color.blue);
								break;
							case drx::enums::MASH::INFERIOR:
								r = (pa.color.red < pb.color.red) ? (pa.color.red) : (pb.color.red);
								g = (pa.color.green < pb.color.green) ? (pa.color.green) : (pb.color.green);
								b = (pa.color.blue < pb.color.blue) ? (pa.color.blue) : (pb.color.blue);
								break;
							case drx::enums::MASH::RED:
								r = pa.color.red / 2 + pb.color.red / 2;
								g = (pa.color.green > pb.color.green) ? (pa.color.green - pb.color.green) : (pb.color.green - pa.color.green);
								b = (pa.color.blue > pb.color.blue) ? (pa.color.blue - pb.color.blue) : (pb.color.blue - pa.color.blue);
								break;
							case drx::enums::MASH::GREEN:
								r = (pa.color.red > pb.color.red) ? (pa.color.red - pb.color.red) : (pb.color.red - pa.color.red);
								g = pa.color.green / 2 + pb.color.green / 2;
								b = (pa.color.blue > pb.color.blue) ? (pa.color.blue - pb.color.blue) : (pb.color.blue - pa.color.blue);
								break;
							case drx::enums::MASH::BLUE:
								r = (pa.color.red > pb.color.red) ? (pa.color.red - pb.color.red) : (pb.color.red - pa.color.red);
								g = (pa.color.green > pb.color.green) ? (pa.color.green - pb.color.green) : (pb.color.green - pa.color.green);
								b = pa.color.blue / 2 + pb.color.blue / 2;
								break;
							}

							p.color._dye(r, g, b);
							p.color.exist = true;
						}
						else {
							if (pa.color.exist) {
								p.color._set(pa.color);
							}
							else if (pb.color.exist) {
								p.color._set(pb.color);
							}
							else {
								p.color._set(X);
							}
						}
						p.x = x;
						p.y = y;
						pixs << p;
					}
					n.pixels << pixs;
					n.draw << pixs;
				}
				a._close();
				b._close();
				n.size.w = w;
				n.size.h = h;

				return n;

			}

			drx::gfx::SPRITE _outline(drx::gfx::COLOR line = B) {
				drx::gfx::SPRITE o = o._center(*this, this->size.w + 2, this->size.h + 2, 1, 1);

				drx::gfx::COLOR tran = X;
				for (DINT x = 1; x < o.size.w - 1; x++) {
					for (DINT y = 1; y < o.size.h - 1; y++) {
						drx::gfx::COLOR c = o.pixels[x][y].color;
						if (c != tran) {
							drx::gfx::PIXEL* u = &o.pixels[x][y - 1];
							drx::gfx::PIXEL* d = &o.pixels[x][y + 1];
							drx::gfx::PIXEL* l = &o.pixels[x - 1][y];
							drx::gfx::PIXEL* r = &o.pixels[x + 1][y];
							if (!u->apply && u->color == tran) u->apply = true;
							if (!d->apply && d->color == tran) d->apply = true;
							if (!l->apply && l->color == tran) l->apply = true;
							if (!r->apply && r->color == tran) r->apply = true;
						}
					}
				}

				for (DINT x = 0; x < o.size.w; x++) {
					for (DINT y = 0; y < o.size.h; y++) {
						if (o.pixels[x][y].apply) {
							o.pixels[x][y].color._set(line);
							o.pixels[x][y].apply = false;
						}
					}
				}

				return o;
			}

			void _draw(SINT px = -1, SINT py = -1, COLOR clr = X) {

				DINT extx = (drx::math::Remainder(this->scale.x) != 0.0) ? (1) : (0);
				DINT exty = (drx::math::Remainder(this->scale.y) != 0.0) ? (1) : (0);
				SINT posx = (px != -1) ? (px) : (this->size.x);
				SINT posy = (py != -1) ? (py) : (this->size.y);
				//this->dim = { px, py, px + (DINT)(this->size.w * this->scale.x), px + (DINT)(this->size.h * this->scale.y) };
				for (DINT x = 0; x < this->size.w; x++) {
					for (DINT y = 0; y < this->size.h; y++) {
						drx::gfx::PIXEL pixel = this->draw[x][y];
						if (pixel.color.exist) {
							DINT s = 0;
							for (DINT i = y + 1; i < this->size.h - 1; i++) {
								bool same = this->draw[x][i].color == pixel.color;
								if (same) s++; else break;
							}
							double pixx = (double)pixel.x;
							double pixy = (double)pixel.y;
							DINT height = (DINT)(this->scale.y * (s + 1) + exty);
							drx::gfx::Set((DINT)(pixx * this->scale.x) + posx, (DINT)(pixy * this->scale.y) + posy, (pixel.color.exist) ? (pixel.color) : (clr), (DINT)this->scale.x + extx, height);
							for (; s > 0; s--) y++;
						}
					}
				}
			}

			void _rotate(double angle, bool center = false) {
				DINT ang = (DINT)angle;
				DINT extx = (drx::math::Remainder(this->scale.x) != 0.0) ? (1) : (0);
				DINT exty = (drx::math::Remainder(this->scale.y) != 0.0) ? (1) : (0);
				for (DINT x = 0; x < this->size.w; x++) {
					for (DINT y = 0; y < this->size.h; y++) {
						drx::gfx::PIXEL pixel = this->draw[x][y];
						if (pixel.color.exist) {
							double pix = 0.0, piy = 0.0;
							DINT dx = 0, dy = 0;
							drx::utils::SPOT px;
							if (center) {
								//pix = (double)(this->size.w / 2 - pixel.x) + 0.1 * this->scale.x;
								//piy = (double)(this->size.h / 2 - pixel.y) + 0.1 * this->scale.y;
								pix = (double)(this->size.w / 2) - (double)pixel.x;
								piy = (double)(this->size.h / 2) - (double)pixel.y;
								px = drx::math::Transform2D({ pix, piy }, { (double)ang }, true);
								dx = (DINT)(px.x * this->scale.x + 0.1 * this->scale.x) + this->size.x + ((this->size.w) * this->scale.x) / 2.0;
								dy = (DINT)(px.y * this->scale.y + 0.1 * this->scale.y) + this->size.y + ((this->size.h) * this->scale.y) / 2.0;
							}
							else {
								pix = (double)pixel.x + 0.1 * this->scale.x;
								piy = (double)pixel.y + 0.1 * this->scale.y;
								px = drx::math::Transform2D({ pix, piy }, { (double)ang }, true);
								dx = (DINT)(px.x * this->scale.x) + this->size.x;
								dy = (DINT)(px.y * this->scale.y) + this->size.y;
							}
							drx::gfx::Set(dx, dy, pixel.color, (DINT)this->scale.x + extx, (DINT)this->scale.y + exty);
						}
					}
				}
			}

			void _dump() {
				std::cout << "\nSPRITE: " << this->size.w << ", " << this->size.h << " (" << this->pixels.length << ")";
				this->draw[0][0]._dump();
				for (DINT x = 0; x < this->pixels.length; x++) {
					for (DINT y = 0; y < this->pixels[x].length; y++) {
						PIXEL pixel = this->pixels[x][y];
						pixel._dump();
					}
				}
			}

			drx::gfx::SPRITE _copy() {
				drx::gfx::SPRITE sprite;
				for (DINT x = 0; x < this->pixels.length; x++) {
					drx::utils::IMAP<drx::gfx::PIXEL> pxs;
					for (DINT y = 0; y < this->pixels.package[x].item.length; y++) {
						drx::gfx::PIXEL p = this->pixels.package[x].item.package[y].item;
						drx::gfx::PIXEL pix;
						pix.x = p.x;
						pix.y = p.y;
						pix.color._set(p.color);
						pix.apply = true;
						pxs << pix;
					}
					sprite.pixels << pxs;
					sprite.draw << pxs;
				}
				sprite.size = this->size;
				sprite.dim = this->dim;
				sprite.name = this->name.text;
				sprite.loaded = this->loaded;
				sprite.scale = this->scale;
				return sprite;
			}

		};
		struct POLYGON {
			POLYGON(double w, double h, double d, double s, const char file[], bool center = true) {
				std::cout << "\n Constructing POLYGON";
				drx::utils::TIME time;
				time._clock(0);
				drx::utils::DATA data;
				data._read(file, 1);
				this->name = file;
				double hx = 0.0, hy = 0.0, hz = 0.0;

				for (DINT e = 0; e < data.exas.length; e++) {
					drx::utils::SPOT spot = data.exas[e]._spot();
					if (spot.x > hx) hx = spot.x;
					if (spot.y > hy) hy = spot.y;
					if (spot.z > hz) hz = spot.z;
					//spot._dump();
				}
				//std::cout << "\n-------------";
				//std::cout << "\nx " << hx << ", y " << hy << ", z " << hz;
				//std::cout << "\n-------------";
				for (DINT e = 0; e < data.exas.length; e++) {
					//data.exas[e]._dump();
					drx::utils::SPOT spot = data.exas[e]._spot();
					if (center) {
						spot.x -= hx / 2.0;
						spot.y -= hy / 2.0;
						spot.z -= hz / 2.0;
					}
					//spot._dump();
					this->spot << spot;
					this->calc << spot;
					this->draw << spot;
					this->multi << spot;
					for (DINT p = 0; p < data.exas[e].pair.length; p++) {
						drx::utils::PAIR pair = { e, data.exas[e].pair[p] };
						this->pair << pair;
					}
					for (DINT g = 0; g < data.exas[e].group.length; g++) {
						DINT grp = data.exas[e].group[g];
						SINT f = -1;
						for (DINT gp = 0; gp < this->group.length; gp++) {
							if (this->group[gp].a == grp) {
								f = gp;
							}
						}
						if (f == -1) {
							drx::utils::GROUP g = { grp, {e} };
							this->group << g;
						}
						else {
							this->group[f].g << e;
						}
					}
				}
				/*
				for (DINT g = 0; g < this->group.length; g++) {
					std::cout << "\n " << g << "(" << this->group[g].a << "):";
					for (DINT c = 0; c < this->group[g].g.length; c++) {
						std::cout << " " << this->group[g].g[c];
					}
				}
				*/
				data.exas._close();
				this->scale = s;
				this->measure = { w, h, d };
				//this->_center();
				this->_size(this->scale);
				time._increment(time);
				std::cout << " " << time._since(1) << "ms.";
				this->debug = false;
			}
			POLYGON(drx::utils::IMAP<drx::utils::SPOT> spots, drx::utils::IMAP<drx::utils::PAIR> pairs, drx::utils::SPOT measure) {
				for (DINT s = 0; s < spots.length; s++) {
					drx::utils::SPOT spot = spots[s];
					this->spot << spot;
					this->calc << spot;
					this->draw << spot;
					this->multi << spot;
				}
				for (DINT p = 0; p < pairs.length; p++) {
					this->pair << pairs[p];
				}
				this->measure = measure;
				this->_size(1.0);
				this->debug = false;
			}
			POLYGON(const char file[], bool center = false) {
				drx::utils::TIME time;
				time._clock(0);
				drx::utils::DATA data;
				bool loaded = data._read(file, 1);
				this->name = file;
				double hx = 0.0, hy = 0.0, hz = 0.0;

				if (center) {
					for (DINT e = 0; e < data.exas.length; e++) {
						drx::utils::SPOT spot = data.exas[e]._spot();
						if (spot.x > hx) hx = spot.x;
						if (spot.y > hy) hy = spot.y;
						if (spot.z > hz) hz = spot.z;
					}
				}
				for (DINT e = 0; e < data.exas.length; e++) {
					drx::utils::SPOT spot = data.exas[e]._spot();
					if (center) {
						spot.x -= hx / 2.0;
						spot.y -= hy / 2.0;
						spot.z -= hz / 2.0;
					}
					this->spot << spot;
					this->calc << spot;
					this->draw << spot;
					this->multi << spot;
					for (DINT p = 0; p < data.exas[e].pair.length; p++) {
						drx::utils::PAIR pair = { e, data.exas[e].pair[p] };
						this->pair << pair;
					}
					drx::utils::GROUP grp;
					grp.a = e;
					for (DINT i = 0; i < data.exas[e].group.length; i++) {
						DINT g = data.exas[e].group[i];
						grp.g << g;
					}
					this->group << grp;
				}

				for (DINT i = 0; i < this->group.length; i++) {
					drx::utils::GROUP g = this->group[i];
					g._dump();
				}

				data.exas._close();
				time._increment(time);
				if (loaded) {
					std::cout << " in " << time._since(1) << " ms.";
				}
				else {
					std::cout << " -- unable to load file.";
				}
				this->fov = 0.0;
				this->scale = 1.0;
				this->radius = 0.0;
				this->debug = false;
			}
			POLYGON() {
				this->fov = 75.0;
				this->scale = 1.0;
				this->radius = 0.0;
				this->debug = false;
			};

			drx::utils::IMAP<drx::utils::SPOT> spot, calc, draw, multi;
			drx::utils::IMAP<drx::utils::FACE> face;
			drx::utils::IMAP<drx::utils::PAIR> pair;
			drx::utils::IMAP<drx::utils::GROUP> group;
			drx::gfx::COLOR color;
			drx::utils::SPOT measure, angle, rotation, direction, position;
			double fov, scale, radius;
			drx::utils::STRING name;
			bool debug;

			void _close() {
				this->spot._close();
				this->calc._close();
				this->draw._close();
				this->multi._close();
				this->pair._close();
				this->face._close();
				this->group._close();
			}

			void _generate(double r = 25.0, DINT s = 3, double ang = 0.0, bool triangle = false) {
				this->radius = r;
				if (s < 3) triangle = false;
				double a = 360.0 / (double)s;
				this->direction = { ang };
				double angle = this->direction.x;
				double gap = drx::utils::Gap(r, 4.0);
				drx::utils::DICE dice;
				this->_close();
				drx::utils::SPOT center = { 0.0 };
				this->_add(center);
				for (DINT i = 0; i < s; i++) {
					double x = sin(drx::math::Rad(angle));
					double y = cos(drx::math::Rad(angle));
					drx::utils::SPOT spot = { r * x, r * y };
					drx::utils::PAIR pair;
					drx::utils::FACE face;
					if (i < s - 1) {
						face.i << 0;
						face.i << i + 1;
						face.i << i + 2;
					}
					else {
						face.i << 0;
						face.i << 1;
						face.i << i + 1;
					}
					this->face << face;
					this->_add(spot);
					angle += a;
				}
				if (this->debug) std::cout << "\nPolygon generated: " << this->face.length;
				if (triangle) {

					for (DINT i = 1; i < this->spot.length; i++) {
						drx::utils::FACE face;
						if (i < this->spot.length - 1) {
							face.i << i;
							face.i << i + 1;
						}
						else {
							face.i << 1;
							face.i << this->spot.length - 1;
						}
						face.i << 0;
						this->face << face;
					}

				}

				//this->_resize({ r, r, 0.0 });
			}

			void _add(drx::utils::SPOT s) {
				this->spot << s;
				this->calc << s;
				this->draw << s;
				this->multi << s;
			}

			void _resize(drx::utils::SPOT size) {
				this->measure = { size.x, size.y, size.z };
				for (DINT s = 0; s < this->spot.length; s++) {
					drx::utils::SPOT* calc = &this->calc[s];
					drx::utils::SPOT spot = this->spot[s];
					calc->x = spot.x * size.x;;
					calc->y = spot.y * size.y;
					calc->z = spot.z * size.z;
				}

			}

			void _scale(double scale) {
				for (DINT s = 0; s < this->spot.length; s++) {
					drx::utils::SPOT spot = this->spot[s];
					this->calc[s].x = spot.x * scale;
					this->calc[s].y = spot.y * scale;
					this->calc[s].z = spot.z * scale;
				}
			}

			void _size(double scale = 0.0) {
				scale = (scale != 0.0) ? (scale) : (this->scale);
				drx::utils::SPOT a;
				for (DINT c = 0; c < this->calc.length; c++) {
					a = this->spot[c];
					//a._dump();
					a.x *= this->measure.x * scale;
					a.y *= this->measure.y * scale;
					a.z *= this->measure.z * scale;
					this->calc[c] = { a.x, a.y, a.z };
					//this->calc[c]._dump();
					//std::cout << "\n------";
				}
			}

			void _position(double x, double y) {
				this->position.x = x;
				this->position.y = y;

			}

			void _draw(SINT x = -1, SINT y = -1, bool spots = true, bool outlines = true, bool triangle = false, bool fill = false) {
				drx::utils::SPOT spot;
				drx::gfx::Set((SINT)this->position.x - 2, (SINT)this->position.y - 2, DG, 4, 4);

				for (DINT s = 0; s < this->calc.length; s++) {
					if (this->calc.package[s].exist) {

						spot = this->calc[s];

						drx::utils::SPOT out = drx::math::Transform3D(spot, this->angle, true);
						out = drx::math::Transform2D(out, this->direction, true);
						this->multi[s] = { out.x, out.y, out.z };
						this->draw[s] = { out.x + this->position.x, out.y + this->position.y, out.z, {this->draw[s].color.red, this->draw[s].color.green, this->draw[s].color.blue} };
						drx::utils::SPOT d = this->draw[s];
						//d._dump();
						if (spots) {
							drx::gfx::Set((DINT)this->draw[s].x - 2, (DINT)this->draw[s].y - 2, { d.color.red, d.color.green, d.color.blue }, 4, 4);
						}
					}
				}
				if (outlines) {
					DINT center = 1;
					for (DINT i = center; i < this->draw.length; i++) {
						drx::utils::SPOT a = this->draw[i];
						drx::utils::SPOT b = (i < this->draw.length - 1) ? (this->draw[i + 1]) : (this->draw[center]);
						//LINE l = { a, b, 1, this->color };
						//l._draw();
					}

					/*
					COLOR color;
					for (DINT p = 0; p < this->pair.length; p++) {
						PAIR pair = this->pair[p];
						SPOT a = this->draw[pair.a], b = this->draw[pair.b];
						SPOT sa = this->multi[pair.a], sb = this->multi[pair.b];
						//if (sa.z > 0.0 || sb.z > 0.0) {
						if (p % 2 == 0) color = R; else color = GR;
						LINE l = { a, b, 2, color };
						l._draw();
						//}
					}
					*/
				}

				if (triangle) {
					drx::gfx::COLOR color = this->color;
					for (DINT f = 0; f < this->face.length; f++) {
						drx::utils::FACE face = this->face[f];
						for (DINT i = 0; i < face.i.length; i++) {
							drx::utils::SPOT a;
							drx::utils::SPOT b;
							if (i == face.i.length - 1) {
								a = this->draw[face.i[i]];
								b = this->draw[face.i[0]];
							}
							else {
								a = this->draw[face.i[i]];
								b = this->draw[face.i[i + 1]];
							}
							if (!color.exist) {
								if (i % 2 == 0) {
									color = R;
								}
								else {
									color = GR;
								}
							}
							//LINE line = { a, b, 1, color };
							//line._draw();
						}
					}
				}

				if (fill) {

					drx::utils::SSPOT scan = true;
					drx::utils::MakePoint(this->draw, &scan);
					for (double y = floor(scan.y); y < scan.h; y += 1.0) {
						double x = floor(scan.x + (scan.w - scan.x) / 2.0);
						double w = x;
						bool moving = true;
						do {
							for (DINT i = 0; i < this->draw.length; i++) {
								drx::utils::SPOT a, b;
								if (i < this->draw.length - 1) {
									a = this->draw[i];
									b = this->draw[i + 1];
								}
								else {
									a = this->draw[i];
									b = this->draw[0];
								}
								drx::utils::SPOT c = { floor(a.x), floor(a.y) }, d = { floor(b.x), floor(b.y) };

								if (drx::utils::OnLineExact(c, d, { x, y })) {
									moving = false;
									break;
								}
							}
							x -= 1.0;
						} while (moving && x > scan.x);
						moving = true;
						do {
							for (DINT i = 0; i < this->draw.length; i++) {
								drx::utils::SPOT a, b;
								if (i < this->draw.length - 1) {
									a = this->draw[i];
									b = this->draw[i + 1];
								}
								else {
									a = this->draw[i];
									b = this->draw[0];
								}
								drx::utils::SPOT c = { floor(a.x), floor(a.y) }, d = { floor(b.x), floor(b.y) };
								if (drx::utils::OnLineExact(c, d, { w, y })) {
									moving = false;
									break;
								}
							}
							w += 1.0;
						} while (moving && w < scan.w);
						double width = w - x;
						drx::gfx::Set((SINT)x, (SINT)y, R, width, 1);
					}
				}

				/*
				for (DINT i = 0; i < this->face.length; i++) {
					FACE face = this->face[i];
					IMAP<SPOT> spots;
					for (DINT i = 0; i < face.i.length; i++) {
						spots << this->draw[face.i[i]];
					}
					SSPOT sorted = {(double)INFINITY, (double)INFINITY, (double)-INFINITY, (double)-INFINITY};
					MATH::_makePoint(spots, &sorted);
					double x = sorted.x;
					double y = sorted.y;
					SPOT s = MATH::_transform3D({ x, y }, this->angle);
					double w = (sorted.w > sorted.x) ? (sorted.w - x) : (x - sorted.w);
					double h = (sorted.h > sorted.y) ? (sorted.h - y) : (y - sorted.h);
					double sw = w / this->texture.base.size.w;
					double sh = h / this->texture.base.size.h;
					this->texture.base.scale = { 1.0, 1.0 };
					this->texture.base.angle = this->angle;
					this->texture.base._transform3D(this->texture.base.scale, x, y);
					//this->texture.base._draw(x (SINT)y);
				}
				*/

				/*
				if (fill && this->face.length > 0) {
					SSPOT r = true;
					MATH::_makePoint(this->draw, &r);
					SSPOT scan = { r.x, r.y, r.w - r.x, r.h - r.y };
					IMAP <SPOT> spots;
					for (DINT f = 0; f < this->face.length; f++) {
						spots._close();
						for (DINT i = 0; i < this->face[f].i.length; i++) {
							spots << this->draw[this->face[f].i[i]];
						}
						for (double y = r.y; y < r.h; y += 1.0) {
							for (double x = r.x; x < r.w; x += 1.0) {
								SPOT p = { x, y };
								if (MATH::_spotInTriangle(p, spots)) {
									state._set((DINT)x, (DINT)y, DG);
								}

							}

						}
					}
					spots._close();

				}
				*/

				if (this->radius > 0.0) {
					//LINE degree = { this->position, {this->position.x + cos(MATH::_rad(this->direction.x)) * this->radius, this->position.y + sin(MATH::_rad(this->direction.x)) * this->radius}, 2, R };
					//degree._draw();
				}

				if (this->position.x != 0.0 && this->position.y != 0.0) {
					//LINE first = { this->position, this->draw[0], 2, G };
					//first._draw();
				}
			}

			void _junction(drx::gfx::POLYGON pol) {
				for (DINT p = 0; p < pol.pair.length; p++) {
					drx::utils::PAIR pair = pol.pair[p];
					pair.a += this->spot.length;
					pair.b += this->spot.length;
					this->pair << pair;
				}
				for (DINT s = 0; s < pol.calc.length; s++) {
					drx::utils::SPOT spot = pol.calc[s];
					bool found = false;
					for (DINT t = 0; t < this->calc.length; t++) {
						if (this->calc[t] == spot) {
							found = true;
							break;
						}
					}
					if (!found) {
						this->spot << spot;
						this->calc << spot;
						this->draw << spot;
						this->multi << spot;
					}
				}

			}

			void _dump() {
				std::cout << "\n\nDumping POLYGON ";
				this->name._dump();
				for (DINT i = 0; i < this->spot.length; i++) {
					this->spot[i]._dump();
					this->calc[i]._dump();
					this->draw[i]._dump();
					std::cout << "\n>>>>";
				}
				this->position._dump();
				std::cout << "\n----------";
				system("pause");

			}


			double _X() {
				double x = (double)INFINITY;
				for (DINT i = 0; i < this->spot.length; i++) {
					if (this->draw[i].x < x) x = this->draw[i].x;
				}
				return x;
			}

			double _W() {
				double w = 0.0;
				for (DINT i = 0; i < this->spot.length; i++) {
					if (this->draw[i].x > w) w = this->draw[i].x;
				}
				return w;
			}

			double _Y() {
				double y = (double)INFINITY;
				for (DINT i = 0; i < this->spot.length; i++) {
					if (this->draw[i].y < y) y = this->draw[i].y;
				}
				return y;
			}

			double _H() {
				double h = 0.0;
				for (DINT i = 0; i < this->spot.length; i++) {
					if (this->draw[i].y > h) h = this->draw[i].y;
				}
				return h;
			}


		};
		struct ANIMATION {
			ANIMATION(std::initializer_list<drx::gfx::SPRITE> sprts, DINT scale = 2) {
				drx::utils::HOLDER<drx::gfx::SPRITE> sprites = sprts;
				//std::cout << "\nConstructing ANIMATION (" << sprites._size() << ")";
				for (DINT s = 0; s < sprites._size(); s++) {
					this->sprite << sprites[s];
				}
				this->size.w = this->sprite[0].size.w;
				this->size.h = this->sprite[0].size.h;
				this->stage = 0;
				this->scale = scale;
				this->facing = 0;
				this->direction = drx::enums::DIR::ASC;
				this->loop = 0;
				this->target = -1;
			}

			ANIMATION(drx::utils::IMAP<drx::gfx::SPRITE> sprites, DINT scale = 1) {
				for (DINT s = 0; s < sprites.length; s++) {
					if (sprites.package[s].exist) {
						this->sprite << sprites[s];
					}
				}
				this->size.w = sprites[0].size.w;
				this->size.h = sprites[0].size.h;
				this->stage = 0;
				this->scale = scale;
				this->facing = 0;
				this->direction = drx::enums::DIR::ASC;
				this->loop = 0;
				this->target = -1;
			}

			ANIMATION() {
				this->stage = 0;
				this->scale = 1;
				this->facing = 0;
				this->direction = drx::enums::DIR::ASC;
				this->loop = 0;
				this->target = -1;
			};
			drx::utils::IMAP<drx::gfx::SPRITE> sprite;
			drx::utils::DIMENSION size, dim;
			DINT stage, scale, facing, loop;
			drx::enums::DIR direction;
			SINT target;

			void _draw(SINT x = -1, SINT y = -1, DINT mirror = 0, DINT outline = 0, DINT duration = 15, SINT stage = -1) {
				if (this->sprite.length > 0) {
					this->dim = { x, y, this->sprite[0].size.w + x, this->sprite[0].size.h + y };
					x = (x != -1) ? (x) : (this->size.x);
					y = (y != -1) ? (y) : (this->size.y);
					if (stage == -1) {
						if (this->sprite.length > 1) {
							if (this->loop >= duration) {
								DINT dir = 0;
								if (this->direction == drx::enums::DIR::ASC) {
									dir = 1;
								}
								else {
									dir = -1;
								}
								this->stage += dir;
								if (this->stage == this->sprite.length - 1) {
									this->direction = drx::enums::DIR::DESC;
								}
								if (this->stage == 0) {
									this->direction = drx::enums::DIR::ASC;
								}
								this->loop = 0;
							}
							this->loop++;
						}
						this->sprite[this->stage]._draw(x, y);
					}
					else {
						this->sprite[stage]._draw(x, y);
					}
				}
			}

			char* _name() {
				return this->sprite[0].name.text;
			}


		};

		void DrawSprite(DINT x, DINT y, SPRITE s) {
			HDC tool = GetDC(drx::gfx::canvas.handle);
			DINT w = (DINT)(s.size.w * s.scale.x);
			DINT h = (DINT)(s.size.h * s.scale.y);
			BitBlt(tool, x, y, w, h, s.temp, 0, 0, SRCCOPY);
			ReleaseDC(drx::gfx::canvas.handle, tool);
		}

		DINT GetRed(drx::gfx::COLOR color) {
			return (color.code >> 16) & 0xff;
		}
		DINT GetGreen(drx::gfx::COLOR color) {
			return (color.code >> 8) & 0xff;
		}
		DINT GetBlue(drx::gfx::COLOR color) {
			return color.code & 0xff;
		}

		void Init(HWND handle, DINT w = 800, DINT h = 600) {
			if (!drx::gfx::canvas.init) {
				drx::gfx::canvas.x = 0;
				drx::gfx::canvas.y = 0;
				drx::gfx::canvas.w = w;
				drx::gfx::canvas.h = h;
				if (handle != NULL) drx::gfx::canvas.handle = handle;
				drx::gfx::canvas.Alloc();
				drx::gfx::canvas.bi.bmiHeader.biSize = sizeof(BITMAPINFO);
				drx::gfx::canvas.bi.bmiHeader.biWidth = drx::gfx::canvas.w;
				drx::gfx::canvas.bi.bmiHeader.biHeight = -drx::gfx::canvas.h;
				drx::gfx::canvas.bi.bmiHeader.biPlanes = 1;
				drx::gfx::canvas.bi.bmiHeader.biBitCount = 32;
				drx::gfx::canvas.bi.bmiHeader.biCompression = BI_RGB;
				drx::gfx::canvas.init = true;
				std::cout << "\nCanvas created. (" << drx::gfx::canvas.w << ", " << drx::gfx::canvas.h << ", " << GetLastError() << ")";
			}
		}
		void Clear(drx::gfx::COLOR color = ENGINE_BACKGROUND) {
			drx::gfx::canvas.pixel = (ULINT*)drx::gfx::canvas.memory;
			for (DINT y = 0; y < drx::gfx::canvas.h; y++) {
				for (DINT x = 0; x < drx::gfx::canvas.w; x++) {
					*drx::gfx::canvas.pixel++ = color.code;
				}
			}
		}
		void Draw() {
			HDC tool = GetDC(drx::gfx::canvas.handle);
			StretchDIBits(tool, drx::gfx::canvas.x, drx::gfx::canvas.y, drx::gfx::canvas.w, drx::gfx::canvas.h, 0, 0, drx::gfx::canvas.w, drx::gfx::canvas.h, drx::gfx::canvas.memory, &drx::gfx::canvas.bi, DIB_RGB_COLORS, SRCCOPY);
			ReleaseDC(drx::gfx::canvas.handle, tool);
		}
		void DrawRectangle(drx::utils::DIMENSION size, drx::gfx::COLOR color = R, bool fill = false, bool centerThickness = true) {
			DINT red = (centerThickness) ? (size.d / 2) : (0);
			if (fill) {
				drx::gfx::Set(size.x - red, size.y - red, color, size.w + red, size.h + red);
			}
			else {
				drx::gfx::Set(size.x - red, size.y - red, color, size.w + red * 2, size.d);
				drx::gfx::Set(size.x - red, size.y - red, color, size.d, size.h + red * 2);
				drx::gfx::Set(size.x - red, size.y + size.h - red, color, size.w + red * 2, size.d);
				drx::gfx::Set(size.x + size.w - red, size.y - red, color, size.d, size.h + red * 2);
			}
		}
		void DrawCircle(drx::utils::SPOT center, double radius, drx::gfx::COLOR color = O, double steps = 1.0, DINT thickness = 1, bool angles = false, double angle = 0.0) {
			DINT red = thickness / 2;
			for (double i = 0.0; i < 360.0; i += steps) {
				double x = center.x + sin(drx::math::Rad(i)) * radius;
				double y = center.y + cos(drx::math::Rad(i)) * radius;
				if (angles) {
					if (i == (DINT)angle) {
						angle += angle;
						drx::gfx::Set((SINT)(x - red), (SINT)(y - red), R, thickness, thickness);
					}
					else {
						drx::gfx::Set((SINT)(x - red), (SINT)(y - red), color, thickness, thickness);
					}
				}
				else {
					drx::gfx::Set((SINT)(x - red), (SINT)(y - red), color, thickness, thickness);
				}
			}
		};
		void FillBackground(SPRITE s) {
			DINT sw = s.size.w * s.scale.x;
			DINT sh = s.size.h * s.scale.y;
			DINT w = drx::gfx::canvas.w / sw;
			DINT h = drx::gfx::canvas.h / sh;

			for (DINT x = 0; x < w + 1; x++) {
				for(DINT y = 0; y < h + 1; y++){
					s._draw(x * sw, y * sh);
				}
			}
		}
	}
	namespace tg {
		struct TEXTUAL {
			TEXTUAL(const char text[], DINT x, DINT y, drx::gfx::COLOR c = B, DINT scale = 2, DINT spacing = 2) {
				this->string = text;
				this->x = x;
				this->y = y;
				this->color = c;
				this->scale = { (double)scale, (double)scale };
				this->spacing = spacing;
				this->display = 1;
				this->id = -1;
			};
			TEXTUAL() {
				this->x = 0;
				this->y = 0;
				//this->color = R;
				this->scale = 1;
				this->spacing = 2;
				this->display = 0;
				this->id = -1;
			};
			drx::utils::STRING string;
			DINT x, y, spacing, display;
			drx::utils::SPOT scale;
			SINT id;
			drx::gfx::COLOR color;

			void _update(const char text[]) {
				this->string = text;
			}
		};
		struct WRITER {
			WRITER(std::initializer_list<drx::gfx::LIT> lts) {
				drx::utils::TIME time;
				time._clock(0);
				drx::utils::HOLDER<drx::gfx::LIT> lits = lts;
				for (DINT i = 0; i < lits._size(); i++) {
					lit << lits[i];
				}
				//dummy = ENGINE_DUMMY;
				dummy = ENGINE_MAP_DUMMY;
				time._increment(time);
				std::cout << "\nConstructed WRITER in " << time._since(1) << " ms.";
				in = false;
			};
			static drx::utils::IMAP<drx::gfx::LIT> lit;
			static drx::utils::IMAP<drx::tg::TEXTUAL> text;
			static drx::utils::IMAP<char> input;
			static bool in;
			static drx::gfx::LIT dummy;
			static drx::utils::STRING write;


			static void _type(const char text[], DINT x, DINT y, DINT size = 2, DINT spacing = 2, drx::gfx::COLOR c = B, bool hr = false, drx::enums::DIRECTION align = drx::enums::DIRECTION::DOWN) {
				DINT px, py, ll = 0;
				drx::gfx::LIT tmp;
				drx::gfx::PIXEL pix;
				DINT height = (hr) ? (7) : (5);
				for (DINT length = 0; text[length] != '\0'; length++) {
					drx::gfx::LITERAL lit = drx::tg::WRITER::_sprite(text[length], hr);
					for (DINT p = 0; p < lit.pixels.length; p++) {
						pix = lit.pixels[p];
						if (pix.color.exist) {
							switch (align) {
							default:
							case drx::enums::DIRECTION::HALT:
							case drx::enums::DIRECTION::UP:
								px = x + (pix.x * size) + (ll * size) + (length * spacing);
								py = y + (pix.y * size);
								break;
							case drx::enums::DIRECTION::CENTER: {
								DINT ext = height - lit.size.h;
								px = x + (pix.x * size) + (ll * size) + (length * spacing);
								py = y + (pix.y * size) + ext;

							} break;
							case drx::enums::DIRECTION::DOWN: {
								DINT ext = height - lit.size.h;
								DINT exti = 0;
								switch (text[length]) {
								default:
									break;
								case '-':
									exti = 1;
								case ':':
								case '+':
								case 'g':
								case 'p':
								case 'q':
								case 'j':
									if (hr) ext = 3 + exti;
									break;
								}
								px = x + (pix.x * size) + (ll * size) + (length * spacing);
								py = y + ((pix.y + ext) * size);
							} break;
							}
							drx::gfx::Set(px, py, c, size, size);
						}
					}
					ll += lit.size.w;
				}
			}

			static drx::gfx::LIT _lit(const char character) {
				for (DINT i = 0; i < lit.length; i++) {
					drx::gfx::LIT a = lit[i];
					if (a.exist) {
						if (a.lower == character || a.upper == character) return a;
					}
				}
				return dummy;
			}

			static drx::gfx::LITERAL _sprite(const char character, bool hr = false) {
				for (DINT i = 0; i < lit.length; i++) {
					drx::gfx::LIT l = lit[i];
					if (l.exist) {
						switch (hr) {
						case true:
							if (l.upper == character) return l.hr;
							if (l.lower == character) return l.hrlc;
							break;
						case false:
							if (l.lower == character || l.upper == character) return l.lr;
							break;
						}
					}
				}
				return {};
			}

			static drx::gfx::LIT _lit(const DINT key) {
				for (DINT i = 0; i < lit.length; i++) {
					drx::gfx::LIT a = lit[i];
					if (a.exist) {
						if (a.key == key) return a;
					}
				}
				return dummy;
			}

			static char _valid(DINT key) {
				for (DINT i = 0; i < lit.length; i++) {
					if (lit[i].key == key) return lit[i].lower;
				}

				return '?';
			}
			static char _c(DINT n) {
				for (DINT l = n; l < WRITER::lit.length; l++) {
					if (WRITER::lit.package[l].exist) {
						drx::gfx::LIT li = WRITER::lit[l];
						if (li.numeric == n) {
							return li.upper;
						}
					}
				}
				return '?';
			}
			static char* _itc(SLINT number = -1) {
				//char write[16];
				write._clear();
				DINT length = 0;
				if (number < 0) {
					drx::tg::WRITER::write.text[0] = '-';
					number = drx::math::Abs(number);
					length++;
				}
				if (number == 0) {
					write.text[0] = '0';
					write.text[1] = '\0';
				}
				else {
					drx::utils::IMAP<DINT> digits;
					DINT count = 0;
					double t = (double)number, calc = 0.0;
					for (; t > 0.91;) {
						t = (double)(number * 0.1);
						number /= 10;
						calc = t - (double)number;
						digits << (DINT)(calc * 10);
					}
					//digits._reverse();			
					for (DINT i = digits.length - 1; i >= 0; i--) {
						write.text[length] = _c(digits[i]);
						length++;
					}
					write.text[length] = '\0';
					digits._close();
				}
				return write.text;
			}

			static char* _dtc(double number = 0.0, DINT decimals = 3) {
				write._clear();
				if (number == 0.0) {
					write._append("0.0");
				}
				else {
					drx::utils::STRING back, front;
					if (number < 0.0) {
						front._append("-");
						number = drx::math::Abs(number);
					}

					DINT f = (DINT)number;
					number = number - (double)f;
					front._append(WRITER::_itc(f));
					front._app('.');
					DINT spacing = 0;
					if (number < 1.0) {
						spacing = 1;
						number += 1.0;
					}
					do {
						number *= 10.0;
					} while (number != trunc(number));

					back._append(WRITER::_itc((SLINT)number));


					write._append(front.text);
					drx::utils::STRING decimaled;
					DINT d = 0;
					do {
						decimaled._app(back[d + spacing]);
						decimals--;
						d++;
					} while (decimals > 0);
					write._append(decimaled.text);

				}
				return write.text;
			}
			static double _ctd(const char text[], DINT decimals = 3) {
				drx::utils::INTC f, b;
				DINT l = 0;
				DINT s = (text[0] == '-') ? (1) : (0);
				drx::utils::STRING n = text;
				for (DINT i = s; text[i] != '.'; i++) {
					f.number << drx::utils::Ctn(text[i]);
					l++;
				}
				n._slice(0, l);
				DINT rounder = 0;
				DINT ll = (s == 1) ? (l + 1) : (l);
				for (DINT i = ll; text[i] != '\0'; i++) {
					DINT n = drx::utils::Ctn(text[i]);
					b.number << drx::utils::Ctn(text[i]);
					rounder++;
				}

				double r = (double)f._number();
				double rb = b._double();// (double)b._number() / MATH::_tnth(1, rounder - 1);
				return (s == 1) ? (-(r + rb)) : (r + rb);
			}
			static SLINT _cti(drx::utils::STRING string) {
				SLINT digits = 0;
				SLINT modifier = 0;
				DINT modder = string.length;
				for (DINT i = 0; i < string.length; i++) {
					drx::gfx::LIT chr = drx::tg::WRITER::_lit(string[i]);
					modifier = drx::utils::Tnth(chr.numeric, modder);
					digits += modifier;
					modder--;
					std::cout << "\n>" << chr.numeric << ", " << modifier;
				}
				digits /= 10;
				std::cout << "\n>>>" << digits;
				return digits;
			}
			static void _draw(drx::tg::TEXTUAL text, drx::gfx::COLOR c = X, bool hr = false) {
				DINT px, py, ll = 0;
				drx::gfx::LIT tmp;
				drx::gfx::PIXEL pix;
				for (DINT length = 0; text.string.text[length] != '\0'; length++) {
					tmp = drx::tg::WRITER::_lit(text.string.text[length]);
					drx::gfx::LITERAL lit;
					switch (hr) {
					case true:
						lit = tmp.hr;
						break;
					case false:
						lit = tmp.lr;
						break;
					}
					for (DINT p = 0; p < lit.pixels.length; p++) {

						pix = lit.pixels[p];
						if (pix.color.exist) {
							if (c.exist) pix.color = c;
							px = text.x + (pix.x * (DINT)text.scale.x) + (ll * (DINT)text.scale.x) + (length * text.spacing);
							py = text.y + (pix.y * (DINT)text.scale.y);
							drx::gfx::Set(px, py, pix.color, (DINT)text.scale.x, (DINT)text.scale.y);
						}
					}
					ll += lit.size.w;
				}
			}
			static void _cin(WPARAM key) {
				if (drx::tg::WRITER::in) {
					char character = drx::tg::WRITER::_valid((DINT)key);
					if (character != '?') drx::tg::WRITER::input << character;
				}
			}
			static drx::utils::STRING _rcin() {
				drx::utils::STRING s;
				for (DINT i = 0; i < drx::tg::WRITER::input.length; i++) {
					s._app(drx::tg::WRITER::input[i]);
				}
				return s;
			}

			static DINT _size(const char text[], DINT spacing = 2, bool hr = false, DINT scale = 1) {
				drx::utils::STRING t = text;
				DINT l = 0;
				for (DINT i = 0; i < t.length; i++) {
					drx::gfx::LIT lit = WRITER::_lit(t[i]);
					switch (hr) {
					case true:
						l += lit.hr.size.w * scale;
						break;
					case false:
						l += lit.lr.size.w * scale;
						break;
					}
					l += spacing;
				}
				return l;
			}

		};
	}
	namespace input {
		struct BUTTON {
			BUTTON() {
				this->down = false;
				this->up = false;
				this->key = -1;
			};
			bool down, up;
			SINT key;
		};

		struct MOUSE {
			MOUSE() {
				this->leave = false;
			};
			drx::input::BUTTON left, middle, right, up, down;
			bool leave;
		};
		struct KEYER {
			KEYER() {};
			drx::input::MOUSE mouse;
			drx::input::BUTTON ctrl, shift, alt, enter, backspace, esc, a, s, d, w;
			drx::utils::IMAP<drx::input::BUTTON> keyboard;

			drx::input::BUTTON operator[] (const char character) {
				drx::gfx::LIT chr = drx::tg::WRITER::_lit(character);
				if (chr.exist) {
					return this->keyboard[chr.key];
				}
				else {
					BUTTON dummy;
					dummy.down = false;
					dummy.up = false;
					return dummy;

				}
			}

			drx::input::BUTTON operator[] (const DINT key) {
				drx::gfx::LIT chr = drx::tg::WRITER::_lit(key);
				if (chr.exist) {
					return this->keyboard[key];
				}
				else {
					BUTTON dummy;
					dummy.down = false;
					dummy.up = false;
					return dummy;
				}
			}

			void _config() {
				this->keyboard = 200;
				this->keyboard._resize();
				for (DINT k = 0; k < drx::tg::WRITER::lit.length; k++) {
					BUTTON b;
					b.key = drx::tg::WRITER::lit[k].key;
					b.up = false;
					b.down = false;
					this->keyboard.package[b.key].item = b;
					this->keyboard.package[b.key].exist = true;
					this->keyboard.package[b.key].z = (double)b.key;
				}
			}

		};

		static drx::input::KEYER keyer;
	}
	namespace view {
		struct WINDOW {
			WINDOW() { };
			static HWND handle;
			static WNDCLASS info;
			static MSG msg;
			static drx::utils::DIMENSION size, client, mouse, mouseLast;
			static bool hasFocus, beingMoved;
			static double freq, refresh;
			static LRESULT CALLBACK WindowEvents(HWND hwnd, UINT uMsg, WPARAM w, LPARAM l);

			void Register(WNDPROC proc, HINSTANCE i, const wchar_t name[]) {
				drx::view::WINDOW::info = {};
				drx::view::WINDOW::info.lpfnWndProc = proc;
				drx::view::WINDOW::info.hInstance = i;
				drx::view::WINDOW::info.lpszClassName = name;
				drx::view::WINDOW::info.hbrBackground = 0; // Note to self handle WM_ERASEBKGND [white stuck]
				drx::view::WINDOW::info.hCursor = 0;
				drx::view::WINDOW::info.style = 0;
				RegisterClass(&drx::view::WINDOW::info);
				std::cout << "\nWindow registered. (" << GetCurrentThreadId() << ", " << GetLastError() << ")";

			};
			void Support() {
				//DISPLAY_DEVICE dd;
				//SLINT res = EnumDisplayDevices(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
				LPCWSTR name = {};
				DEVMODE dm;
				SLINT r = EnumDisplaySettings(name, ENUM_CURRENT_SETTINGS, &dm);
				drx::view::WINDOW::freq = 30.0; // (double)dm.dmDisplayFrequency;
				drx::view::WINDOW::refresh = 1000.0 / drx::view::WINDOW::freq;

			}
			void Create(DINT w, DINT h, DINT x, DINT y, HINSTANCE i, const wchar_t name[], bool fullscreen = false) {

				this->Register(drx::view::WINDOW::WindowEvents, i, name);
				this->Support();
				if (fullscreen) {
					RECT rect;
					GetClientRect(GetDesktopWindow(), &rect);
					drx::view::WINDOW::handle = CreateWindow(drx::view::WINDOW::info.lpszClassName, name, WS_BORDER, 0, 0, rect.right, rect.bottom, NULL, NULL, drx::view::WINDOW::info.hInstance, NULL);
					SetWindowLong(drx::view::WINDOW::handle, GWL_STYLE, 0);
				}
				else {
					drx::view::WINDOW::handle = CreateWindow(drx::view::WINDOW::info.lpszClassName, name, WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, drx::view::WINDOW::info.hInstance, NULL);
				}
				if (drx::view::WINDOW::handle == NULL) {
					std::cout << "\nWindow handle null";
					system("pause");
				}
				else {
					std::cout << "\nWindow created. (" << drx::view::WINDOW::handle << ", " << GetLastError() << ")";
					ShowWindow(drx::view::WINDOW::handle, SW_NORMAL);
					RECT rect;
					if (fullscreen) {
						GetWindowRect(drx::view::WINDOW::handle, &rect);
						drx::view::WINDOW::size = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
					}
					else {
						drx::view::WINDOW::size = { x, y, w, h };
						GetClientRect(drx::view::WINDOW::handle, &rect);

					}
					drx::view::WINDOW::client = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
				}
			}

		};
		struct STATISTICS {
			static DINT FPS;
			static DINT SleepLimit;
			static std::chrono::duration<double, std::milli> lasted;
			static std::chrono::steady_clock::time_point start, finish, last, next, high;
			static double elapsed, timer;
			static DINT seconds, loops;

			static void LimitFPS() {
				if (drx::view::STATISTICS::lasted.count() < drx::view::STATISTICS::SleepLimit) {
					DINT ms = (double)drx::view::STATISTICS::SleepLimit - drx::view::STATISTICS::lasted.count();
					Sleep(ms);
				}
			}
			static drx::utils::STRING ViewFPS() {
				drx::utils::STRING fps;
				SLINT loops = drx::view::STATISTICS::loops;
				SLINT seconds = drx::view::STATISTICS::seconds;
				if(loops > 0 && seconds > 0) fps = drx::tg::WRITER::_itc(loops / seconds);
				return fps;
			}
		};

		drx::view::WINDOW window;

		bool MouseHovering(drx::utils::DIMENSION measure, drx::utils::SPOT scale = {1.0, 1.0}, bool debug = false) {
			if(debug) drx::gfx::Set(measure.x, measure.y, R, measure.w * scale.x, measure.h * scale.y);
			if (drx::view::window.mouse.x >= measure.x && drx::view::window.mouse.x <= measure.x + measure.w * scale.x) {
				if (drx::view::window.mouse.y >= measure.y && drx::view::window.mouse.y <= measure.y + measure.h * scale.y) {
					return true;
				}
			}
			return false;
		}

		struct MENU {
			MENU(drx::gfx::COLOR bg = B, drx::gfx::COLOR text = W, DINT scale = 2) {
				this->bg = bg;
				this->text = text;
				this->scale = scale;
				this->padding = 8;
				this->margin = 5;
				this->width = 0;
				this->hr = false;
				this->spacing = 2;
				this->height = 5;
				this->highlighted = false;
			};
			struct ITEM {
				ITEM(const char name[] = "", SINT order = -1) {
					this->name = name;
					this->order = order;
					this->highlighted = false;
					this->subHeight = 0;
					this->subWidth = 0;
					this->color = W;
					this->height = 0;
				};
				struct SUBITEM {
					SUBITEM(const char name[] = "", SINT order = -1) {
						this->name = name;
						this->order = order;
						this->color = W;
						this->highlighted = false;
					};
					drx::utils::STRING name;
					SINT order;
					drx::utils::DIMENSION pos;
					drx::gfx::COLOR color;
					bool highlighted;
					drx::gfx::SPRITE icon;
				};

				SINT order;
				drx::utils::STRING name;
				drx::utils::DIMENSION pos;
				DINT height, subWidth, subHeight;
				drx::utils::IMAP<drx::view::MENU::ITEM::SUBITEM> item;
				bool highlighted;
				drx::gfx::COLOR color;

				void operator << (const char name[]) {
					drx::view::MENU::ITEM::SUBITEM item = { name, this->item.length };
					item.pos = { this->pos.x, this->pos.y + this->height * (this->item.length + 1) };
					DINT w = drx::tg::WRITER::_size(name, 2, false, 2);
					if (w > this->subWidth) this->subWidth = w;
					this->item << item;
				}
			};
			drx::utils::DIMENSION pos;
			DINT scale, padding, margin, spacing, width, height;
			bool hr, highlighted;
			drx::gfx::COLOR bg, text;
			drx::utils::IMAP<drx::view::MENU::ITEM> item;


			void _draw() {
				drx::gfx::Set(this->pos.x, this->pos.y, this->bg, this->pos.w, this->pos.h);
				for (DINT j = 0; j < this->item.length; j++) {
					drx::view::MENU::ITEM* item = &this->item[j];
					DINT height = 0;
					switch (this->hr) {
					case true:
						height = 7;
						break;
					case false:
						height = 5;
						break;
					}
					drx::utils::DIMENSION d = { item->pos.x - this->padding, item->pos.y - this->padding, item->pos.w + this->padding, height * this->scale + this->padding * 2};
					drx::utils::SPOT s = { 1.0, 1.0 };
					if (drx::view::MouseHovering(d, s)) {
						item->highlighted = true;
						item->color = R;
						this->highlighted = true;
					}
					drx::tg::WRITER::_type(item->name.text, item->pos.x, item->pos.y, this->scale, this->spacing, item->color, this->hr, drx::enums::DIRECTION::DOWN);

					if (item->highlighted) {

						drx::gfx::Set(item->pos.x - this->padding,item->pos.y + item->pos.h + this->padding, MENU_VIEW, item->subWidth + this->padding * 2, item->item.length * item->subHeight + item->item.length * this->padding + this->padding);

						bool subLighted = false;
						for (DINT i = 0; i < item->item.length; i++) {
							drx::view::MENU::ITEM::SUBITEM* sub = &item->item[i];
							sub->highlighted = false;
							sub->color = this->text;
							drx::utils::DIMENSION d = { sub->pos.x - this->padding, sub->pos.y, item->subWidth + this->padding * 2, height * this->scale + this->padding};
							if (drx::view::MouseHovering(d, s)) {
								sub->highlighted = true;
								sub->color = R;
								subLighted = true;

							}
							drx::tg::WRITER::_type(sub->name.text, sub->pos.x, sub->pos.y + this->padding, this->scale, this->spacing, sub->color, this->hr, drx::enums::DIRECTION::DOWN);
							sub->icon._draw();
						}

						if ((drx::view::window.mouse.x < item->pos.x || drx::view::window.mouse.x > item->pos.x + item->pos.w + this->padding) || (drx::view::window.mouse.y < item->pos.y - this->padding || drx::view::window.mouse.y > item->pos.y + height * this->scale + this->padding)) {
							if (!subLighted) {
								item->highlighted = false;
								item->color = W;
								this->highlighted = false;
							}
						}
					}
				}
			}

			void operator << (const char text[]) {
				drx::utils::STRING t = text;
				drx::view::MENU::ITEM item = { t.text, this->item.length };
				item.pos = { this->width + this->padding, this->padding };
				DINT w = drx::tg::WRITER::_size(text, this->spacing, this->hr, this->scale);
				item.pos.w = w;
				item.pos.h = (this->hr) ? (7 * this->scale) : (5 * this->scale);
				this->width += w + this->padding * 2;
				this->item << item;
			}

			void _addSub(const char name[], DINT order, drx::gfx::SPRITE icon = {}) {
				drx::view::MENU::ITEM* item = &this->item[order];
				drx::view::MENU::ITEM::SUBITEM sub = { name, item->item.length };
				DINT w = drx::tg::WRITER::_size(name, this->spacing, this->hr, this->scale);
				item->subHeight = (this->hr) ? (7 * this->scale) : (5 * this->scale);
				if (w > item->subWidth) {
					item->subWidth = w;
				}
				item->item << sub;
				for (DINT i = 0; i < item->item.length; i++) {
					item->item[i].pos.w = item->subWidth;
				}
				drx::view::MENU::ITEM::SUBITEM* s = &item->item[item->item.length - 1];
				s->pos = { item->pos.x, this->height + (item->pos.h + this->padding) * (item->item.length - 1), item->subWidth, item->subHeight };
				if (icon.size.w > 0 && icon.size.h > 0) {
					s->icon = icon;
					s->icon._position((DINT)(s->pos.x + s->pos.w - icon.size.w * icon.scale.x), (DINT)(s->pos.y + this->padding + 4));
				}
			}
		};

		void Init(DINT x, DINT y, HINSTANCE inst, const char name[], bool fullscreen, DINT w, DINT h) {
			drx::utils::STRING title = name;
			drx::view::window.Create(w, h, x, y, inst, title.wtext, fullscreen);
		}
	}
	namespace file {

		struct DRX {
			DRX() {};
			std::fstream file, file2;
			drx::utils::STRING name, name2;
			void _open(drx::utils::STRING file, bool out = true, DINT type = 1, bool brackets = false) {
				switch (type) {
				default:
					break;
				case 0:
					file._append(".sdrx");
					break;
				case 1:
					file._append(".pdrx");
					break;
				case 2:
					file._append(".sdrxb");
					break;
				}

				if (out) {
					if (!brackets) {
						this->name = ENGINE_MAP_DIRECTORY_SPRITE_OUTPUT;
						this->name._app('\\');
						this->name._append(file.text);
						this->file.open(this->name.text, std::fstream::out);
					}
					else {
						this->name2 = ENGINE_MAP_DIRECTORY_BRACKETS_OUTPUT;
						this->name2._app('\\');
						this->name2._append(file.text);
						this->file2.open(this->name2.text, std::fstream::out);
						if (this->file2.is_open()) {
							std::cout << "\n> " << this->name2.text << " open.";
						}
						else {
							std::cout << "\n> Failed to open: " << this->name2.text;
						}
					}
				}
				else {
					this->name = file.text;
					this->file.open(this->name.text, std::fstream::in);
				}
			}
		};

		struct SDRX : drx::file::DRX {

			drx::gfx::SPRITE sprite;

			void _load(drx::utils::STRING file, bool spr = false, bool brkts = false) {
				std::cout << "\nLoading SPRITE " << file.text;
				drx::utils::STRING path = ENGINE_MAP_DIRECTORY_SPRITE;
				if (path.length > 0) path._app('\\');
				path._append(file.text);
				this->_open(path, false, 0, brkts);
				if (this->file.is_open()) {
					drx::utils::TIME time;
					time._clock(0);
					drx::utils::TIME t2;
					t2._clock(0);
					DINT l = 0;
					drx::utils::INTC colors, w, h;
					DINT clr = 0;
					char line[128];
					drx::utils::TIME timeRead;
					timeRead._clock(0);
					drx::utils::IMAP<drx::gfx::PIXEL> pixels;
					DINT mo = 1;
					while (!this->file.eof()) {
						this->file.getline(line, 128);
						switch (line[0]) {
						default:
							break;
						case 'c': {
							drx::gfx::PIXEL p;
							SINT cl = -1;
							for (DINT i = 1; line[i] != '-'; i++) {
								if (cl == 5) break;
								if (line[i] != ' ') {
									colors << (SLINT)drx::utils::Ctn(line[i]);
									switch (cl) {
									default:
										break;
									case 0:
										p.color.red = (DINT)colors._number();
										break;
									case 1:
										p.color.green = (DINT)colors._number();
										break;
									case 2:
										p.color.blue = (DINT)colors._number();
										break;
									case 3:
										switch ((DINT)colors._number()) {
										default:
											p.color.exist = false;
											break;
										case 0:
											p.color.exist = false;
											break;
										case 1:
											p.color.exist = true;
											break;
										}
										break;
									case 4:
										switch ((DINT)colors._number()) {
										default:
											p.color.brk = false;
											break;
										case 0:
											p.color.brk = false;
											break;
										case 1:
											p.color.brk = true;
											break;
										}
										break;

									}
								}
								else {
									cl++;
									colors._close();
								}
							}
							pixels << p;
						} break;
						case 'w': {
							DINT i = 0;
							do {
								if (line[i] != ' ') {
									w << (SLINT)drx::utils::Ctn(line[i]);
								}
								i++;
							} while (line[i] != '-');
						} break;
						case 'h': {
							DINT i = 0;
							do {
								if (line[i] != ' ') {
									h << (SLINT)drx::utils::Ctn(line[i]);
								}
								i++;
							} while (line[i] != '-');
						} break;
						}
					}
					timeRead._increment(timeRead);
					//std::cout << " Read in ";
					//timeRead._took(true, true);

					DINT width = (DINT)w._number();
					DINT height = (DINT)h._number();

					for (DINT i = 0; i < width; i++) {
						drx::utils::IMAP<drx::gfx::PIXEL> ip;
						this->sprite.pixels << ip;
						this->sprite.draw << ip;
					}

					this->sprite.size.w = width;
					this->sprite.size.h = height;
					this->sprite.scale = { 1.0, 1.0 };

					t2._increment(t2);

					SINT cl = 0;
					DINT x = 0, y = 0;
					drx::utils::TIME t4;
					t4._clock(0);
					for (DINT px = 0; px < pixels.length; px++) {
						drx::gfx::PIXEL* pix = &pixels[px];
						pix->x = x;
						pix->y = y;
						this->sprite.pixels[x] << *pix;
						this->sprite.draw[x] << *pix;
						y++;
						if (y == height) {
							y = 0;
							x++;
						}
					}
					t4._increment(t4);
					this->sprite.loaded = true;
					this->sprite.name = file.text;
					this->sprite.temp = CreateCompatibleDC(GetDC(drx::view::window.handle));
					time._increment(time);
					std::cout << " (" << this->sprite.temp << ")";
					std::cout << " in ";
					time._took();
				}
				else {
					std::cout << " > Unable to open " << this->name.text;
					std::cout << "\n" << std::filesystem::current_path();
				}
			}

			void _close() {
				this->file.close();
				this->sprite.pixels._close();
			}

			void operator << (drx::gfx::COLOR color) {
				if (this->file.is_open()) {
					this->file << "c ";

					this->file << drx::tg::WRITER::_itc(color.red);
					this->file << " ";
					this->file << drx::tg::WRITER::_itc(color.green);
					this->file << " ";
					this->file << drx::tg::WRITER::_itc(color.blue);

					if (!color.exist) this->file << " " << '0'; else this->file << " " << '1';
					if (!color.brk) this->file << " " << '0'; else this->file << " " << '1';
					this->file << "-\n";
				}

				if (this->file2.is_open()) {
					this->file2 << '{';
					this->file2 << drx::tg::WRITER::_itc(color.red);
					this->file2 << ',';
					this->file2 << drx::tg::WRITER::_itc(color.green);
					this->file2 << ',';
					this->file2 << drx::tg::WRITER::_itc(color.blue);
					this->file2 << ',';
					this->file2 << drx::tg::WRITER::_itc(color.exist);
					this->file2 << ',';
					this->file2 << drx::tg::WRITER::_itc(color.brk);
					this->file2 << '}';
				}
			}

			void _size(DINT w, DINT h) {
				if (this->file.is_open()) {
					this->file << "w " << drx::tg::WRITER::_itc(w) << "-\n";
					this->file << "h " << drx::tg::WRITER::_itc(h) << "-\n";
				}
				if (this->file2.is_open()) {
					this->file2 << " " << drx::tg::WRITER::_itc(w);
					this->file2 << ", " << drx::tg::WRITER::_itc(h);
				}

			}

		};

		struct PDRX : DRX {

			drx::gfx::POLYGON polygon;

			void _load(drx::utils::STRING file, bool center = true, bool face = false, bool brkts = false) {
				std::cout << "\nLoading POLYGON " << file.text;
				drx::utils::TIME time;
				time._clock(0);
				drx::utils::STRING path = ENGINE_MAP_DIRECTORY_POLYGON;
				if (path.length > 0) path._app('\\');
				path._append(file.text);
				this->_open(path, false, 1, brkts);
				if (this->file.is_open()) {
					file._strip(file.length - 5, file.length - 1);
					this->polygon.name = file.text;
					char line[128];
					bool faceFound = false;
					while (!this->file.eof()) {
						this->file.getline(line, 128);
						switch (line[0]) {
						default:
							break;
						case 'n': {
							for (DINT i = 1; line[i] != '='; i++) {
								//if (line[i] != ' ') this->polygon.name._app(line[i]);
							}
						} break;
						case 's': {
							drx::utils::SPOT s;
							SINT cl = -1;
							drx::utils::STRING x, y, z, d;
							for (DINT i = 1; line[i] != '='; i++) {
								if (line[i] == ' ') {
									switch (cl) {
									default:
										break;
									case 0:
										x = d.text;
										break;
									case 1:
										y = d.text;
										break;
									case 2:
										z = d.text;
										break;
									}

									d._clear();
									cl++;
								}
								else {
									d._app(line[i]);
								}
							}

							s = { drx::tg::WRITER::_ctd(x.text), drx::tg::WRITER::_ctd(y.text), drx::tg::WRITER::_ctd(z.text) };

							this->polygon.spot << s;
							this->polygon.calc << s;
							this->polygon.draw << s;
							this->polygon.multi << s;
							//std::cout << "\nx: " << x.text << " y: " << y.text << " z: " << z.text;
						} break;
						case 'p': {
							drx::utils::PAIR pair;
							SINT cl = -1;
							for (DINT i = 1; line[i] != '='; i++) {
								if (line[i] == ' ') {
									cl++;
								}
								else {
									switch (cl) {
									default:
										break;
									case 0:
										pair.a = drx::utils::Ctn(line[i]);
										break;
									case 1:
										pair.b = drx::utils::Ctn(line[i]);
										break;

									}
								}
							}
							this->polygon.pair << pair;
						} break;
						case 'f': {
							faceFound = true;
							if (!face) {
								drx::utils::FACE face;
								drx::utils::INTC n;
								for (DINT i = 2; line[i] != '='; i++) {
									if (line[i] == ' ') {
										face.i << (DINT)n._number();
										n._close();
									}
									else {
										n.number << drx::utils::Ctn(line[i]);
									}
								}
								this->polygon.face << face;
							}
						} break;
						}

					}

					if (!faceFound && face) {
						for (DINT i = 0; i < this->polygon.spot.length - 2; i++) {
							drx::utils::FACE face;
							face.i << i;
							face.i << i + 1;
							face.i << i + 2;
							this->polygon.face << face;

						}


					}

					if (center) {
						drx::utils::SSPOT values = true;
						drx::utils::MakePoint(this->polygon.spot, &values);

						for (DINT i = 0; i < this->polygon.spot.length; i++) {
							drx::utils::SPOT* s = &this->polygon.spot[i];
							s->x -= values.w / 2.0;
							s->y -= values.h / 2.0;
							s->z -= values.d / 2.0;
						}
					}


					time._increment(time);
					std::cout << " in ";
					time._took();
				}
				else {
					std::cout << " > Unable to open " << path.text;
				}
			};


			void _close() {
				this->file.close();
				this->polygon.spot._close();
				this->polygon.draw._close();
				this->polygon.calc._close();
				this->polygon.multi._close();
				this->polygon.pair._close();
				this->polygon.group._close();
				this->polygon.face._close();
			};

		};

	}
	namespace lib {

		struct PREFIX {
			PREFIX(const char name[]) {
				this->name._write(name);
			};
			PREFIX() {};
			drx::utils::STRING name;
		};

		struct SUFFIX {
			SUFFIX(const char name[]) {
				this->name._write(name);
			};
			SUFFIX() {};
			drx::utils::STRING name;
		};

		struct CODEX {
			CODEX(bool loaded) {
				if (loaded) {
					//srand((unsigned int)time(NULL));
					drx::utils::TIME time;
					time._clock(0);
					this->_build();
					sprites = ENGINE_MAP_DIRECTORY_SPRITE;
					polygons = ENGINE_MAP_DIRECTORY_POLYGON;
					bsprites = ENGINE_MAP_DIRECTORY_BRACKETS;
					base = ENGINE_MAP_BASE_POOL;
					prefix = ENGINE_MAP_PREFIX_POOL;
					suffix = ENGINE_MAP_SUFFIX_POOL;

					time._increment(time);
					std::cout << "\nConstructed CODEX in " << time._since(1) << "ms.";
					this->loaded = loaded;
				}
			};
			CODEX() {
				this->loaded = false;
			}
			struct DUMMY {
				DUMMY() {
					//this->move = this->animation;
				};
				drx::gfx::SPRITE sprite, tile;
				drx::gfx::ANIMATION animation;
				//MOVE move;
			};
			bool loaded;
			DUMMY dummy;
			static drx::utils::DIRECTORY sprites, polygons, bsprites;
			static drx::utils::IMAP<drx::gfx::SPRITE> sprite, bsprite;
			static drx::utils::IMAP<drx::gfx::ANIMATION> animation;
			static drx::utils::IMAP<drx::gfx::POLYGON> polygon;

			//static CHART<MOVE> move;
			//static CHART<ABILITY> ability;
			static drx::utils::IMAP<drx::lib::PREFIX> prefix;
			static drx::utils::IMAP<drx::lib::SUFFIX> suffix;
			static drx::utils::IMAP<drx::utils::STRING> base;

			static drx::gfx::SPRITE _sprite(const char name[]) {
				drx::gfx::SPRITE s = drx::lib::CODEX::sprite[_search(name)];
				return s;
			}
			static SINT _search(const char name[]) {
				//std::cout << "\nSearching SPRITE: " << name << " (" << sprites.file.length << ")";
				for (DINT s = 0; s < drx::lib::CODEX::sprite.length; s++) {
					if (drx::lib::CODEX::sprite.package[s].exist) {
						//std::cout << "\n> " << sprite[s].name.text << " / " << name;
						if (drx::lib::CODEX::sprite[s].name._exact(name)) return s;
					}
				}
				for (DINT f = 0; f < drx::lib::CODEX::sprites.file.length; f++) {
					bool sfound = 0;
					drx::utils::STRING file = sprites.file[f];
					if (file._match(name)) {
						DINT l = 0;
						bool b = false;
						if (file._match("sdrx")) {
							l = 5;
							b = true;
						}
						else {
							l = 4;
						}
						file._slice(file.length - l, file.length - 1);
						if (file._exact(name)) {
							if (b) {
								drx::file::SDRX sdrx;
								sdrx._load(name);
								sprite << sdrx.sprite;
							}
							else {
								return -1;
							}
							//_animate(file);
							return sprite.length - 1;
						}
					}
				}
				return -1;
			}
			static SINT _asearch(const char name[]) {
				for (DINT a = 0; a < animation.length; a++) {
					if (animation.package[a].exist) {
						drx::utils::STRING n = animation[a]._name();
						if (n._match(name)) return a;
					}
				}

				SINT sprite = drx::lib::CODEX::_search(name);
				if (sprite >= 0) {
					drx::utils::IMAP<drx::gfx::SPRITE> ss;
					for (DINT i = 0; i < sprites.file.length; i++) {
						drx::utils::STRING file = sprites.file[i];
						if (file._match(name)) {
							DINT l = 0;
							if (file._match("sdrx")) {
								l = 5;
							}
							else {
								l = 4;
							}
							file._slice(file.length - l, file.length - 1);
							sprite = drx::lib::CODEX::_search(file.text);
							if (sprite >= 0) ss << drx::lib::CODEX::sprite[sprite];
						}
					}

					drx::gfx::ANIMATION animation;// = ss;
					drx::lib::CODEX::animation << animation;
					ss._close();
					return drx::lib::CODEX::animation.length - 1;
				}
				return -1;
			}
			static SINT _psearch(const char name[]) {
				for (DINT s = 0; s < polygon.length; s++) {
					if (polygon.package[s].exist) {
						if (polygon[s].name._match(name)) return s;
					}
				}
				for (DINT f = 0; f < polygons.file.length; f++) {
					bool sfound = 0;
					drx::utils::STRING file = polygons.file[f];
					bool match = file._match(".pdrx");
					if (file._match(name)) {
						file._slice(file.length - 5, file.length - 1);
						drx::file::PDRX pdrx;
						pdrx._load(file);
						//POLYGON pol = { file.text, true };
						polygon << pdrx.polygon;
						return polygon.length - 1;
					}
				}


				return -1;

			}
			void _build() {

				sprite << this->dummy.sprite;
				animation << this->dummy.animation;
				//move << this->dummy.move;
			}
			void _dump() {
				std::cout << "\nSprites: ";
				for (DINT s = 0; s < sprite.length; s++) {
					std::cout << "\n> " << sprite[s].name.text;
				}
				std::cout << "\nAnimationws: ";
				for (DINT s = 0; s < animation.length; s++) {
					std::cout << "\n> " << animation[s]._name();
				}
				/*
				std::cout << "\nMoves: ";
				for (DINT s = 0; s < move.length; s++) {
					std::cout << "\n> " << move[s].name.text;
				}
				*/
				std::cout << "\nPrefixes: ";
				for (DINT s = 0; s < prefix.length; s++) {
					std::cout << "\n>" << prefix[s].name.text;
				}
				std::cout << "\nSuffixes: ";
				for (DINT s = 0; s < suffix.length; s++) {
					std::cout << "\n>" << suffix[s].name.text;
				}

			}

		};

	}
	namespace measuring {
		template <class TimeType = std::chrono::milliseconds, class ClockType = std::chrono::steady_clock>
		struct TIMER {
			TIMER() {
				this->s = ClockType::now();
				this->e = ClockType::now();
			};
			using TimePoint = typename ClockType::time_point;
			TimePoint s, e;
			double Stamp() {
				auto ms = this->e - this->s;
				return ms.count();
			}

			void Start() {
				this->s = ClockType::now();
			}
			void End() {
				this->e = ClockType::now();
			}
		};
	}

	struct RUNNER {
		private:
			drx::gfx::STATE canvas;
			double timer;
			drx::measuring::TIMER<std::chrono::nanoseconds, std::chrono::steady_clock> tmr;
		public:
			static bool running;
			static drx::utils::STRING title;
			static DINT sec;
			static double etimer;

		void Setup(DINT x, DINT y, HINSTANCE inst, const char name[], bool fullscreen = false, DINT w = 800, DINT h = 600, bool console = true, SINT seedRand = -1) {
			drx::RUNNER::title = name;
			drx::view::Init(x, y, inst, name, fullscreen, w, h);
			drx::gfx::Init(drx::view::window.handle, drx::view::window.client.w, drx::view::window.client.h);
			if (console) ShowWindow(GetConsoleWindow(), SW_SHOW); else ShowWindow(GetConsoleWindow(), SW_HIDE);
			if (seedRand >= 0) srand(seedRand); else srand((unsigned int)time(NULL));
			drx::utils::STRING s = ENGINE_MAP_DIRECTORY_SOURCES;
			for (; s._strip('.', '/');) std::filesystem::current_path(std::filesystem::current_path().parent_path());
			if (s.length > 0) std::filesystem::current_path(s.text);
			drx::tg::WRITER(ENGINE_TYPOGRAPH);
			drx::lib::CODEX(true);
			drx::input::keyer._config();
			this->Load();
			drx::view::STATISTICS::FPS = 60;
			drx::view::STATISTICS::SleepLimit = 1000 / drx::view::STATISTICS::FPS;
			drx::RUNNER::running = true;
		}
		void Refresh() {
			POINT* m = new POINT;
			if (GetCursorPos(m)) {
				if (ScreenToClient(drx::view::window.handle, m)) {
					m->x = (m->x < 0) ? (0) : (m->x);
					m->y = (m->y < 0) ? (0) : (m->y);
					SINT x = drx::view::window.mouse.x - (DINT)m->x;
					SINT y = drx::view::window.mouse.y - (DINT)m->y;

					//drx::window.mouse.x -= x;
					//drx::window.mouse.y -= y;
					//if (drx::view::window.mouse.x != (DINT)m->x || drx::view::window.mouse.y != (DINT)m->y) {
					drx::view::window.mouseLast.x = drx::view::window.mouse.x;
					drx::view::window.mouseLast.y = drx::view::window.mouse.y;
					drx::view::window.mouse.x = (DINT)m->x;
					drx::view::window.mouse.y = (DINT)m->y;
					//}
				}
			}
			delete m;
		}
		void Run() {
			do{
				this->tmr.Start();
				while (PeekMessage(&drx::view::WINDOW::msg, drx::view::WINDOW::handle, 0, 0, PM_REMOVE)) {
					TranslateMessage(&drx::view::WINDOW::msg);
					DispatchMessage(&drx::view::WINDOW::msg);
				}
				this->Refresh();
				this->Update();
				if (this->timer >= drx::view::window.refresh) {
					this->Draw();
					drx::gfx::Draw();
					this->timer -= drx::view::window.refresh;
				}
				this->Clean();
				this->tmr.End();
				drx::view::STATISTICS::elapsed = this->tmr.Stamp() / 1000000.0;
				drx::view::STATISTICS::timer += drx::view::STATISTICS::elapsed;
				this->timer += drx::view::STATISTICS::elapsed;
				drx::RUNNER::etimer += drx::view::STATISTICS::elapsed;
				drx::view::STATISTICS::loops++;
				if (drx::view::STATISTICS::timer >= 1000.0) {
					drx::view::STATISTICS::timer -= 1000.0;
					drx::view::STATISTICS::seconds++;
					//drx::RUNNER::sec++;
					drx::utils::STRING title;
					title._append(drx::RUNNER::title.text);
					title._space(drx::view::STATISTICS::ViewFPS().text);
					drx::view::STATISTICS::loops = 0;
					drx::view::STATISTICS::seconds = 0;
					SetWindowText(drx::view::window.handle, title.wtext);
					//std::cout << "\nLooping, " << drx::view::STATISTICS::elapsed;
				}
			} while (drx::RUNNER::running);
		}
		void Clean() {
			for (DINT i = 0; i < drx::input::keyer.keyboard.size; i++) {
				if(drx::input::keyer.keyboard.package[i].exist) drx::input::keyer.keyboard[i].up = false;
			}
			drx::input::keyer.a.up = false;
			drx::input::keyer.s.up = false;
			drx::input::keyer.w.up = false;
			drx::input::keyer.d.up = false;
			drx::input::keyer.alt.up = false;
			drx::input::keyer.ctrl.up = false;
			drx::input::keyer.enter.up = false;
			drx::input::keyer.esc.up = false;
			drx::input::keyer.backspace.up = false;
			drx::input::keyer.shift.up = false;
			drx::input::keyer.mouse.left.up = false;
			drx::input::keyer.mouse.right.up = false;
			drx::input::keyer.mouse.middle.up = false;
			drx::input::keyer.mouse.up.up = false;
			drx::input::keyer.mouse.down.up = false;
			drx::input::keyer.mouse.leave = false;
		}

		virtual void Load() {};
		virtual void Update() {};
		virtual void Draw() {};
	};

}

LRESULT CALLBACK drx::view::WINDOW::WindowEvents(HWND window, UINT msg, WPARAM w, LPARAM l) {
	LRESULT result = 0;
	switch (msg) {
	case WM_KEYDOWN: {
		//std::cout << "\n> KEY: " << w << ", " << l;
		drx::input::keyer.keyboard[(DINT)w].down = true;
		drx::input::keyer.keyboard[(DINT)w].up = false;
		switch (w) {
		default:
			break;
		case 8:
			drx::input::keyer.backspace.down = true;
			drx::input::keyer.backspace.up = false;
			break;
		case 13:
			drx::input::keyer.enter.down = true;
			drx::input::keyer.enter.up = false;
			break;
		case 16:
			drx::input::keyer.shift.down = true;
			drx::input::keyer.shift.up = false;
			break;
		case 17:
			drx::input::keyer.ctrl.down = true;
			drx::input::keyer.ctrl.up = false;
			break;
		case 18:
			drx::input::keyer.alt.down = true;
			drx::input::keyer.alt.up = false;
			break;
		case 65:
			drx::input::keyer.a.down = true;
			drx::input::keyer.a.up = false;
			break;
		case 68:
			drx::input::keyer.d.down = true;
			drx::input::keyer.d.up = false;
			break;
		case 83:
			drx::input::keyer.s.down = true;
			drx::input::keyer.s.up = false;
			break;
		case 87:
			drx::input::keyer.w.down = true;
			drx::input::keyer.w.up = false;
			break;
		case VK_ESCAPE:
			drx::input::keyer.esc.down = true;
			drx::input::keyer.esc.up = false;
			break;
		}


	} break;
	case WM_KEYUP: {
		drx::input::keyer.keyboard[(DINT)w].down = false;
		drx::input::keyer.keyboard[(DINT)w].up = true;
		switch (w) {
		default:
			break;
		case 8:
			drx::input::keyer.backspace.down = false;
			drx::input::keyer.backspace.up = true;
			break;
		case 13:
			drx::input::keyer.enter.down = false;
			drx::input::keyer.enter.up = true;
			break;
		case 16:
			drx::input::keyer.shift.down = false;
			drx::input::keyer.shift.up = true;
			break;
		case 17:
			drx::input::keyer.ctrl.down = false;
			drx::input::keyer.ctrl.up = true;
			break;
		case 18:
			drx::input::keyer.alt.down = false;
			drx::input::keyer.alt.up = true;
			break;
		case 65: {
			drx::input::keyer.a.down = false;
			drx::input::keyer.a.up = true;
		} break;
		case 68: {
			drx::input::keyer.d.down = false;
			drx::input::keyer.d.up = true;
		} break;
		case 83: {
			drx::input::keyer.s.down = false;
			drx::input::keyer.s.up = true;
		} break;
		case 87: {
			drx::input::keyer.w.down = false;
			drx::input::keyer.w.up = true;
		} break;
		case VK_ESCAPE:
			drx::input::keyer.esc.down = false;
			drx::input::keyer.esc.up = true;
			break;
		}
		drx::tg::WRITER::_cin(w);
		std::cout << "\n>" << w;
	} break;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY: {
		drx::RUNNER::running = false;
	} break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(window, &rect);
		drx::gfx::canvas.Resize(rect.right - rect.left, rect.bottom - rect.top);
		drx::view::window.size = { rect.top, rect.left, drx::gfx::canvas.w, drx::gfx::canvas.h };
	} break;
	case WM_MOVING: {
		drx::view::window.beingMoved = true;
	} break;
	case WM_MOVE: {
		drx::view::window.beingMoved = false;
	} break;
	case WM_RBUTTONDOWN: {
		drx::input::keyer.mouse.right.down = true;
	} break;
	case WM_RBUTTONUP: {
		drx::input::keyer.mouse.right.down = false;

	} break;
	case WM_LBUTTONDOWN: {
		drx::input::keyer.mouse.left.down = true;
		drx::input::keyer.mouse.left.up = false;
	} break;
	case WM_LBUTTONUP: {
		drx::input::keyer.mouse.left.down = false;
		drx::input::keyer.mouse.left.up = true;
		POINT* m = new POINT;
		if (GetCursorPos(m)) {
			if (ScreenToClient(drx::view::window.handle, m)) {
				m->x = (m->x < 0) ? (0) : (m->x);
				m->y = (m->y < 0) ? (0) : (m->y);
			}
		}
		delete m;
	} break;
	case WM_MOUSEWHEEL: {
		SINT dir = GET_WHEEL_DELTA_WPARAM(w);
		if (dir == -120) {
			// down
			drx::input::keyer.mouse.up.up = true;
		}
		if (dir == 120) {
			// up
			drx::input::keyer.mouse.down.up = true;
		}

	} break;
	case WM_MOUSELEAVE:
		drx::input::keyer.mouse.leave = true;
		break;
	case WM_ACTIVATE:
		drx::view::window.hasFocus = true;
		break;
	case WM_KILLFOCUS:
		drx::view::window.hasFocus = false;
		break;
	case WM_ERASEBKGND:
		//result = DefWindowProc(window, msg, w, l);
		return 1;
		break;
	case WM_PAINT:{
		/*
		RECT rc;
		GetClientRect(drx::view::window.handle, &rc);
		int canvas_width = rc.right;
		int canvas_height = rc.bottom;

		PAINTSTRUCT ps;
		auto hdc = BeginPaint(drx::view::window.handle, &ps);

		//create memory dc:
		auto memdc = CreateCompatibleDC(hdc);
		auto hbmp = CreateCompatibleBitmap(hdc, canvas_width, canvas_height);
		auto oldbmp = SelectObject(memdc, hbmp); //<- memdc is ready

		//draw on memory dc:
		BITMAPINFOHEADER bi{ sizeof(bi), drx::gfx::canvas.w, drx::gfx::canvas.h, 1, 32, BI_RGB };
		SetDIBitsToDevice(memdc, 0, 0, drx::gfx::canvas.w, drx::gfx::canvas.h, 0, 0, 0, drx::gfx::canvas.h, drx::gfx::canvas.pixel,
			(BITMAPINFO*)&bi, DIB_RGB_COLORS);

		//draw on actual dc:
		BitBlt(hdc, 0, 0, canvas_width, canvas_height, memdc, 0, 0, SRCCOPY);

		//clean up:
		SelectObject(memdc, oldbmp);
		DeleteObject(hbmp);
		DeleteDC(memdc);
		EndPaint(drx::view::window.handle, &ps);
		*/
		return DefWindowProc(window, msg, w, l);
		} break;
	default: {
		result = DefWindowProc(window, msg, w, l);
	} break;
	}
	return result;
}

HWND drx::view::WINDOW::handle;
WNDCLASS drx::view::WINDOW::info;
MSG drx::view::WINDOW::msg;
drx::utils::DIMENSION drx::view::WINDOW::size, drx::view::WINDOW::client, drx::view::WINDOW::mouse, drx::view::WINDOW::mouseLast;
bool drx::view::WINDOW::hasFocus, drx::view::WINDOW::beingMoved;
bool drx::RUNNER::running;

std::chrono::steady_clock::time_point drx::view::STATISTICS::last, drx::view::STATISTICS::next, drx::view::STATISTICS::high, drx::view::STATISTICS::start, drx::view::STATISTICS::finish;
std::chrono::duration<double, std::milli> drx::view::STATISTICS::lasted;
double drx::view::STATISTICS::timer, drx::view::STATISTICS::elapsed, drx::view::WINDOW::freq, drx::view::WINDOW::refresh, drx::RUNNER::etimer;
DINT drx::view::STATISTICS::loops, drx::view::STATISTICS::seconds, drx::RUNNER::sec;
DINT drx::view::STATISTICS::FPS, drx::view::STATISTICS::SleepLimit;

drx::utils::IMAP<drx::gfx::LIT> drx::tg::WRITER::lit;
drx::utils::IMAP<drx::tg::TEXTUAL> drx::tg::WRITER::text;
drx::utils::IMAP<char> drx::tg::WRITER::input;
bool drx::tg::WRITER::in;
drx::gfx::LIT drx::tg::WRITER::dummy;
drx::utils::STRING drx::tg::WRITER::write;

drx::utils::STRING drx::RUNNER::title;

drx::utils::DIRECTORY drx::lib::CODEX::sprites, drx::lib::CODEX::polygons, drx::lib::CODEX::bsprites;
drx::utils::IMAP<drx::gfx::SPRITE> drx::lib::CODEX::sprite, drx::lib::CODEX::bsprite;
drx::utils::IMAP<drx::gfx::ANIMATION> drx::lib::CODEX::animation;
drx::utils::IMAP<drx::gfx::POLYGON> drx::lib::CODEX::polygon;
drx::utils::IMAP<drx::lib::PREFIX> drx::lib::CODEX::prefix;
drx::utils::IMAP<drx::lib::SUFFIX> drx::lib::CODEX::suffix;
drx::utils::IMAP<drx::utils::STRING> drx::lib::CODEX::base;

