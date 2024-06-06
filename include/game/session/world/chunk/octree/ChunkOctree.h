#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <glm/vec2.hpp>
#include <utility>

// Todo implement json serialize/deserialize
struct JSON {

};

/**
* Octree for storing voxels in a chunk
*/
class ChunkOctree {

	template <int RootBounds, int ArrayMax = 16>
	struct OctreeBounds {
		constexpr OctreeBounds() : values()
		{
			uint16_t maxDepth = 0;
			while (true) {
				auto value = (RootBounds + 1) / 2 >> (maxDepth + 1);
				maxDepth++;
				// reached max depth
				if (value == 1) {
					break;
				}
			}

			for (uint16_t depth = 0; depth < maxDepth; depth++) {
				auto value = (RootBounds + 1) / 2 >> (depth + 1);
				if (value <= UINT16_MAX) {
					values[depth] = value;
				}
				else {
					values[depth] = 1;
				}
			}
		}
		uint8_t values[ArrayMax];
	};


	// concept - serializable
	template <typename NodeData>
	class Node {
	public:

		using bounds_t = std::pair<glm::tvec2<uint8_t>, glm::tvec2<uint8_t>>;

		constexpr static uint8_t MAX_BOUNDS_N = UINT8_MAX;

		constexpr static Node::bounds_t MAX_BOUNDS = { {0,0}, {MAX_BOUNDS_N, MAX_BOUNDS_N} };

		constexpr static uint8_t MAX_DEPTH = 16;

		constexpr static uint8_t MAX_CHILDREN = 8;

		constexpr static OctreeBounds<16*16> boundsTable;

	private:

		Node* _parent = nullptr;

		const uint8_t _depth = 0;

		const uint8_t _corner = 0;
		
		const uint16_t _bounds;

		std::map<uint8_t, std::shared_ptr<Node>> _children{};

		NodeData _data{};

		constexpr uint8_t getBoundsForDepth(uint8_t depth) {
			return boundsTable.values[depth];
		}

	public:
		
		/**
		* Construct an empty node
		*/
		Node(NodeData data, Node* parent, uint8_t depth, uint8_t corner):
			_data(data), 
			_parent(parent), 
			_depth(depth),
			_corner(corner),
			_bounds(getBoundsForDepth(depth))
		{}

		/**
		* Construct root node from serialized data
		*/
		Node(JSON& data) :_parent(nullptr), _depth(0), _corner(0), _bounds(getBoundsForDepth(0))
		{
		// deserialize data and children
		}

		std::shared_ptr<Node> getChild(uint8_t index) {
			if (_children.count(index)) {
				return _children.at(index);
			}
		}

		void addChildAtIndex(NodeData data, uint8_t index) 
		{
			_children[index] = std::make_shared<Node>(data, this, _depth + 1, getCornerPointForIndex(index));
		}
		
		JSON serialize() const {
			return {};
		}
	};

	struct VoxelNodeData {};

	std::shared_ptr<Node<VoxelNodeData>> _rootNode;

public:

	ChunkOctree();

	const std::shared_ptr<Node>& rootNode() const;

	JSON serialize() const {
		return _rootNode->serialize();
	}

	void deserialize(const JSON& data) {
		_rootNode = std::make_unique<Node>(nullptr, 0, data);
	}

	void test() {
		_rootNode->getChild
	}




};