#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

template <typename Type>
class SingleLinkedList {
	struct Node {
		Node() = default;

		Node(const Type& val, Node* next)
			: value(val)
			  , next_node(next) {}

		Type value;
		Node* next_node = nullptr;
	};

	template <typename ValueType>
	class BasicIterator {
		friend class SingleLinkedList;

		explicit BasicIterator(Node* node) {
			node_ = node;
		}

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

		BasicIterator() = default;

		BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

		BasicIterator& operator=(const BasicIterator& rhs) = default;

		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ != rhs.node_;
		}

		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
			return node_ != rhs.node_;
		}

		BasicIterator& operator++() noexcept {
			node_ = node_->next_node;
			return *this;
		}

		BasicIterator operator++(int) noexcept {
			auto previous = *this;
			++*this;
			return previous;
		}

		[[nodiscard]] reference operator*() const noexcept {
			return node_->value;
		}

		[[nodiscard]] pointer operator->() const noexcept {
			return &node_->value;
		}


		Node* node_ = nullptr;
	};

public:
	using Iterator = BasicIterator<Type>;
	using ConstIterator = BasicIterator<const Type>;

	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;

	SingleLinkedList() = default;

	SingleLinkedList(std::initializer_list<Type> values) {
		InitFromForwardRange(values.begin(), values.end());
	}

	SingleLinkedList(const SingleLinkedList& other) {
		InitFromForwardRange(other.begin(), other.end());
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		if (this != &rhs) {
			auto temp(rhs);
			swap(temp);
		}
		return *this;
	}

	void swap(SingleLinkedList& other) noexcept {
		std::swap(head_.next_node, other.head_.next_node);
		std::swap(size_, other.size_);
	}

	~SingleLinkedList() {
		Clear();
	}

	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator(&head_);
	}

	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return ConstIterator(const_cast<Node*>(&head_));
	}

	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator(const_cast<Node*>(&head_));
	}

	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		Node* pos_node = pos.node_;
		Node* new_node = new Node(value, pos_node->next_node);
		pos_node->next_node = new_node;
		++size_;
		return Iterator(new_node);
	}

	void PopFront() noexcept {
		auto next = head_.next_node;
		head_.next_node = next->next_node;
		--size_;
		delete next;
	}

	Iterator EraseAfter(ConstIterator pos) noexcept {
		Node* node_to_delete = pos.node_->next_node;
		pos.node_->next_node = node_to_delete->next_node;
		delete node_to_delete;
		--size_;
		return Iterator(pos.node_->next_node);
	}

	size_t GetSize() const noexcept {
		return size_;
	}

	bool IsEmpty() const noexcept {
		return size_ <= 0;
	}

	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);
		size_++;
	}

	void Clear() noexcept {
		while (size_ > 0) {
			Node* next = head_.next_node->next_node;
			delete head_.next_node;
			head_.next_node = next;
			size_--;
		}
	}

	[[nodiscard]] Iterator begin() noexcept {
		return Iterator(head_.next_node);
	}

	[[nodiscard]] Iterator end() noexcept {
		return Iterator(nullptr);
	}

	[[nodiscard]] ConstIterator begin() const noexcept {
		return ConstIterator(head_.next_node);
	}

	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator(nullptr);
	}

	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return ConstIterator(head_.next_node);
	}

	[[nodiscard]] ConstIterator cend() const noexcept {
		return ConstIterator(nullptr);
	}

private:
	Node head_ = Node();
	size_t size_ = 0;

	void InitFromForwardRange(auto begin, auto end) {
		std::vector<Type> temp(begin, end);
		for (auto rit = temp.rbegin(); rit != temp.rend(); ++rit) {
			PushFront(*rit);
		}
	}
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs < rhs);
}
