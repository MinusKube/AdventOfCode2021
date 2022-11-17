#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "day16.h"

struct packet {
    int version_number;
    int type_id;

    long long value;

    std::vector<packet> children;
};

static int read_bit(std::stringstream& stream) {
    char c;
    stream >> c;

    return c == '1';
}

static int read_number(std::stringstream& stream, int bit_count) {
    int result = 0;

    for (int i = 0; i < bit_count; i++) {
        int bit = read_bit(stream);
        result |= (bit << (bit_count - i - 1));
    }

    return result;
}

static packet read_packet(std::stringstream& stream) {
    packet p;
    p.version_number = read_number(stream, 3);
    p.type_id = read_number(stream, 3);

    if (p.type_id == 4) {
        // Literal Value Packet

        long long value = 0;

        bool is_last_group = false;
        while (!is_last_group) {
            is_last_group = !read_bit(stream);

            value <<= 4;
            value |= read_number(stream, 4);
        }

        p.value = value;
    }
    else {
        // Operator Packet

        int length_type = read_bit(stream);

        if (length_type == 0) {
            int total_length = read_number(stream, 15);
            int target_pos = (int) stream.tellg() + total_length;

            while (stream.tellg() < target_pos) {
                packet new_p = read_packet(stream);
                p.children.push_back(std::move(new_p));
            }
        }
        else if (length_type == 1) {
            int packet_count = read_number(stream, 11);

            for (int i = 0; i < packet_count; i++) {
                packet new_p = read_packet(stream);
                p.children.push_back(std::move(new_p));
            }
        }

        if (p.type_id == 0) {
            long long sum = 0;
            for (packet& child : p.children) {
                sum += child.value;
            }
            p.value = sum;
        }
        else if (p.type_id == 1) {
            long long product = 1;
            for (packet& child : p.children) {
                product *= child.value;
            }
            p.value = product;
        }
        else if (p.type_id == 2) {
            long long minimum = std::numeric_limits<int>::max();
            for (packet& child : p.children) {
                minimum = std::min(minimum, child.value);
            }
            p.value = minimum;
        }
        else if (p.type_id == 3) {
            long long maximum = std::numeric_limits<int>::min();
            for (packet& child : p.children) {
                maximum = std::max(maximum, child.value);
            }
            p.value = maximum;
        }
        else if (p.type_id == 5) {
            p.value = (p.children[0].value > p.children[1].value);
        }
        else if (p.type_id == 6) {
            p.value = (p.children[0].value < p.children[1].value);
        }
        else if (p.type_id == 7) {
            p.value = (p.children[0].value == p.children[1].value);
        }
    }

    return p;
}

static int compute_version_sum(packet p) {
    int result = p.version_number;

    for (packet child : p.children) {
        result += compute_version_sum(child);
    }

    return result;
}

void day16_run() {
    std::ifstream file("../data/day16.txt");

    std::stringstream stream;

    char c;
    while (file >> c) {
        switch(c)
        {
        case '0': stream << "0000"; break;
        case '1': stream << "0001"; break;
        case '2': stream << "0010"; break;
        case '3': stream << "0011"; break;
        case '4': stream << "0100"; break;
        case '5': stream << "0101"; break;
        case '6': stream << "0110"; break;
        case '7': stream << "0111"; break;
        case '8': stream << "1000"; break;
        case '9': stream << "1001"; break;
        case 'A': stream << "1010"; break;
        case 'B': stream << "1011"; break;
        case 'C': stream << "1100"; break;
        case 'D': stream << "1101"; break;
        case 'E': stream << "1110"; break;
        case 'F': stream << "1111"; break;
        }
    }

    stream.clear();
    stream.seekg(0);

    auto packet = read_packet(stream);

    int p1_result = compute_version_sum(packet);
    std::cout << "Part 1 Result: " << p1_result << std::endl;

    long long p2_result = packet.value;
    std::cout << "Part 2 Result: " << p2_result << std::endl;
}
