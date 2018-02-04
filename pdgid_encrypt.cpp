#define _CRT_SECURE_NO_WARNINGS

#include <map>
#include <string>
#include <string.h>
#include <algorithm>
#include <stdio.h>

std::map<std::string, char> id2char;
std::multimap<char, std::string> const char2id = {
	{ 'a', "20113"},
	{ 'b', "5"},
	{ 'c', "4"},
	{ 'd', "1"},
	{ 'e', "11"},
	{ 'f', "225"},
	{ 'g', "21" },
	{ 'g', "39" },
	{ 'h', "25"},
	{ 'i', "715"}, // my birthday 
	{ 'j', "443"},
	{ 'k', "130"},
	{ 'k', "310" },
	{ 'k', "311" },
	{ 'k', "321" },
	{ 'l', "1020"},
	{ 'm', "3222"},
	{ 'm', "3212" },
	{ 'm', "3112" },
	{ 'n', "2112" },
	{ 'n', "111" },
	{ 'n', "211" },
	{ 'n', "221" },
	{ 'n', "331" },
	{ 'o', "2224"},
	{ 'o', "2214" },
	{ 'o', "2114" },
	{ 'o', "1114" },
	{ 'p', "2212"},
	{ 'q', "113"},
	{ 'q', "213" },
	{ 'r', "553"},
	{ 's', "3"},
	{ 't', "6"},
	{ 'u', "2"},
	{ 'v', "12"},
	{ 'v', "14" },
	{ 'v', "16" },
	{ 'w', "24" },
	{ 'w', "223"},
	{ 'x', "445"},
	{ 'y', "22" },
	{ 'z', "23"},

};

void init_id2char() {
	for (auto &p : char2id) {
		id2char[p.second] = p.first;
	}
}

void encryption(char const *str, uint32_t &hash) {
	// djb2 hash algorithm
	for (; *str; ++str) {
		char c = *str;
		hash = ((hash << 5) + hash) + (unsigned char)c;
		bool upper = false;
		if (c <= 'Z' && c >= 'A') {
			c = c + ('a' - 'A');
			upper = true;
		}
		if (c == '#') {
			printf("##");
		} else if (c <= 'z' && c >= 'a') {
			auto p = char2id.equal_range(c);
			size_t n = std::distance(p.first, p.second);
			size_t ith = hash % n;
			std::advance(p.first, ith);
			printf(upper ? "#0%s" : "#%s", p.first->second.c_str());
			if (*(str + 1) <= '9' && *(str + 1) >= '0') {
				// in case u224 -> #2224 -> o
				printf("#.");
			}
		} else {
			printf("%c", c);
		}
	}
}

void decryption(char const *str) {

	for (; *str; ) {
		char const *sharp = str;
		if (*str == '#') {
			if (*(str + 1) == '#') {
				// double ## is a #
				printf("#");
				str += 2;
			} else if (*(str + 1) == '.') {
				// empty char
				str += 2;
			} else {
				++str;
				bool upper = false;
				if (*str == '0') {
					str++;
					upper = true;
				}
				std::string id;
				int i = 0;
				for (; i < 6; ++i) {
					if (*str <= '9' && *str >= '0') {
						id.push_back(*str);
						str++;
					} else {
						break;
					}
				}

				auto fd = id2char.find(id);
				if (fd == id2char.end()) {
					std::string rng(sharp, str - sharp);
					printf("%s", rng.c_str());
				} else {
					printf("%c", upper ? fd->second - 'a' + 'A' : fd->second);
				}


			}

		} else {
			printf("%c", *str);
			++str;
		}
	}

}

std::string read_file(FILE *f) {
	std::string ret;

	char buf[100];
	for (;;) {
		size_t nread = fread(buf, 1, 100, f);
		if (nread > 0) {
			ret.append(buf, buf + nread);
		} else {
			break;
		}
	}
	return ret;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("usage: `pdgid_enctrypt <public_string>` for encryption and `pdgid_enctrypt -d <secret_string>` for dencryption\n");
		printf("       use `-f <file.txt>` to read a file");
		return 0;
	}

	if (std::string("-d") == argv[1]) {
		// decryption
		init_id2char();
		for (int i = 2; i < argc; ++i) {
			char *str = argv[i];
			if (strcmp(str, "-f") == 0) {
				++i;
				char *file = argv[i];
				if (FILE *f = fopen(file, "r")) {
					auto fstr = read_file(f);
					decryption(fstr.c_str());
					fclose(f);
				} else {
					printf("<open `%s` failed>", file);
				}
			} else {
				decryption(str);
			}
			if(i+1 != argc) printf(" ");
		}

	} else {
		uint32_t hash = 5381;
		for (int i = 1; i < argc; ++i) {
			char *str = argv[i];

			if (strcmp(str, "-f") == 0) {
				++i;
				char *file = argv[i];
				if (FILE *f = fopen(file, "r")) {
					auto fstr = read_file(f);
					encryption(fstr.c_str(), hash);
					fclose(f);
				} else {
					printf("<open `%s` failed>", file);
				}
			} else {
				encryption(str, hash);
			}
			if (i + 1 != argc) printf(" ");

		}
	}

}
