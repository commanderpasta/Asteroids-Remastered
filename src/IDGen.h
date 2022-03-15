#pragma once

inline unsigned int idCount = 1;

inline int generateId() {
	return ++idCount;
}
