
test: build/VulkanTest
	./build/VulkanTest

shaders:
	glslc shaders/shaders.vert -o shaders/vert.spv
	glslc shaders/shaders.frag -o shaders/frag.spv

clean_shaders:
	rm -f shaders/*.spv

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./build/VulkanTest
