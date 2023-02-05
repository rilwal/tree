#pragma once

#include <vector>
#include <memory>
#include "glm.hpp"


struct Branch {
	Branch(glm::vec2 origin, glm::vec2 direction, float length, float width);
	glm::vec2 origin, direction;
	float length, width;

	void add_child();
	std::vector<std::unique_ptr<Branch>> children;
};


void Branch::add_child() {
	children.push_back(std::make_unique<Branch>(glm::vec2(origin + direction * (std::rand() * length)), glm::vec2(std::rand(), std::rand()), width / 2, length / 2));

}


void treee() {
	Branch trunk(glm::vec2(0, 0), glm::vec2(0, 1), 100, 500);
	trunk.add_child();
	trunk.add_child();
	trunk.add_child();

	trunk.children[0]->add_child();
}
