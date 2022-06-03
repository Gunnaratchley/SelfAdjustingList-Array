#ifndef SELF_ADJUSTING_LIST_H
#define SELF_ADJUSTING_LIST_H

#include <algorithm>
#include <iostream>

namespace nwacc {
	template <typename T>
	class linked_list {
	private:
		/**
		 * Constructs a node struct to create a doubly linked list.
		 */
		struct node {

			T data;

			node* previous;

			node* next;

			node(const T& data = T{ }, node* previous = nullptr, node* next = nullptr)
				: data{ data }, previous{ previous }, next{ next } { }

			node(T&& data, node* previous = nullptr, node* next = nullptr)
				: data{ std::move(data) }, previous{ previous }, next{ next } { }
		};

	public:
		class const_iterator {
		public:

			/**
			 * Constructor for const iterator.
			 */
			const_iterator() : current{ nullptr }
			{ }
			/**
			 * Returns the T stored at the current position.
			*/
			const T& operator*() const
			{
				return this->retrieve();
			}
			/**
			 * Overload of ++ operator to work with const iterator.
			*/
			const_iterator& operator++()
			{
				// remember we have value++ and ++value
				// We have two method for pre or post increment.
				this->current = this->current->next;
				return *this;
			}
			/**
			 * Overload of ++ operator to work with const iterator.
			*/
			const_iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}
			/**
			 * Overload of -- operator to work with const iterator.
			*/
			const_iterator& operator--()
			{
				this->current = this->current->previous;
				return *this;
			}
			/**
			 * Overload of -- operator to work with const iterator.
			*/
			const_iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}
			/**
			 * Overload of == operator for comparison.
			*/
			bool operator==(const const_iterator& rhs) const
			{
				return this->current == rhs.current;
			}
			/**
			 * Overload of != operator for comparison.
			*/
			bool operator!=(const const_iterator& rhs) const
			{
				return !(*this == rhs);
			}

		protected:
			node* current;

			// Protected helper in const_iterator that returns the T
			// stored at the current position. Can be called by all
			// three versions of operator* without any type conversions.
			T& retrieve() const
			{
				return this->current->data;
			}

			// Protected constructor for const_iterator.
			// Expects a pointer that represents the current position.
			const_iterator(node* position) : current{ position }
			{ }

			friend class linked_list<T>;
		};

		class iterator : public const_iterator {
		public:

			// Public constructor for iterator.
			iterator()
			{ }

			T& operator*()
			{
				return const_iterator::retrieve();
			}
			/**
			 * Return the T stored at the current position.
			 */
			const T& operator*() const
			{
				return const_iterator::operator*();
			}
			/**
			 * Overloaded ++ operator to work with iterator.
			 */
			iterator& operator++()
			{
				this->current = this->current->next;
				return *this;
			}
			/**
			 * Overloaded ++ operator to work with iterator.
			 */
			iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}
			/**
			 * Overloaded -- operator to work with iterator.
			 */
			iterator& operator--()
			{
				this->current = this->current->previous;
				return *this;
			}
			/**
			 * Overloaded -- operator to work with iterator.
			 */
			iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

		protected:
			// Protected constructor for iterator.
			// Expects the current position.
			iterator(node* position) : const_iterator{ position }
			{ }

			friend class linked_list<T>;
		};

	public:
		linked_list()
		{
			this->init();
		}

		~linked_list()
		{
			this->clear();
			delete this->head;
			delete this->tail;
		}
		/**
		 *.
		*/
		linked_list(const linked_list& rhs)
		{
			this->init();
			// Notice here, I can use a foreach loop because I have an iterator!
			for (auto& value : rhs) {
				this->push_back(value);
			}
		}

		linked_list& operator=(const linked_list& rhs)
		{
			auto copy = rhs;
			std::swap(*this, copy);
			return *this;
		}

		linked_list(linked_list&& rhs)
			: my_size{ rhs.my_size }, head{ rhs.head }, tail{ rhs.tail }
		{
			rhs.my_size = 0;
			rhs.head = nullptr;
			rhs.tail = nullptr;
		}

		linked_list& operator=(linked_list&& rhs)
		{
			std::swap(this->my_size, rhs.my_size);
			std::swap(this->head, rhs.head);
			std::swap(this->tail, rhs.tail);
			return *this;
		}

		/**
		 * Return iterator representing beginning of list
		 */
		iterator begin()
		{
			return iterator(this->head->next);
		}

		/**
		 * Return iterator representing beginning of list
		 */
		const_iterator begin() const
		{
			return const_iterator(this->head->next);
		}

		/**
		 * Return iterator representing end marker of list
		 */
		iterator end()
		{
			// This is a C++ thing. Begin in c++ starts always
			// at the first element with a value.
			// end always ends after the last element. 
			return iterator(this->tail);
		}
		/**
		* Returns a const_iterator to the tial of the list.
		*/
		const_iterator end() const
		{
			return const_iterator(this->tail);
		}

		/**
		* Returns size of the list.
		*/
		int size() const
		{
			return this->my_size;
		}
		/**
		* Checks if list is empty.
		*/
		bool empty() const
		{
			return this->size() == 0;
		}
		/**
		* Clears the list.
		*/
		void clear()
		{
			while (!this->empty()) {
				this->pop_front();
			}
		}

		// front, back, push_front, push_back, pop_front, and pop_back
		// are the basic double-ended queue operations.
		T& front()
		{
			return *this->begin();
		}
		/**
		* Returns value of the begining of the list.
		*/
		const T& front() const
		{
			return *this->begin();
		}
		/**
		* Returns value of the end of the list.
		*/
		T& back()
		{
			return *--this->end();
		}
		/**
		* Returns const value of the end of the list.
		*/
		const T& back() const
		{
			return *--this->end();
		}
		/**
		 * Adds a new element at the end of the list, after its current last element.
		 *
		 * @param value the value to add to the list.
		 */
		void push_front(const T& value)
		{
			this->insert(this->begin(), value);
		}
		/**
		 * Adds a new element at the end of the list, after its current last element.
		 *
		 * @param value the value to add to the list.
		 */
		void push_back(const T& value)
		{
			// Remember, we are inserting BEFORE the given iterator, so end here is fine. 
			this->insert(this->end(), value);
		}
		/**
		 * Adds a new element at the front of the list.
		 *
		 * @param value the value to add to the list.
		*/
		void push_front(T&& value)
		{
			this->insert(this->begin(), std::move(value));
		}
		/**
		 * Adds a new element at the end of the list, after its current last element.
		 *
		* @param value the value to add to the list.
		*/
		void push_back(T&& value)
		{
			this->insert(this->end(), std::move(value));
		}
		/**
		 * Erases element at the begining of the list.
		*/
		void pop_front()
		{
			this->erase(this->begin());
		}
		/**
		* Erases element at the end of the list.
		*/
		void pop_back()
		{
			this->erase(--this->end());
		}
		/**
		 * Adds a new node at the end of the list, after its current node.
		 *
		 * @param value the value to place in the node.
		 */	
		iterator insert(iterator position, const T& value)
		{
			// I need the current node in the iterator.
			// This is a pointer to the node of the iterator. 
			auto* current_node = position.current;
			this->my_size++;
			return  iterator(
				current_node->previous = current_node->previous->next =
				new node{ value, current_node->previous, current_node });
		}
		/**
		 * Adds a new node at the end of the list, after its current node.
		 *
		 * @param value the value to place in the node.
		 */
		iterator insert(iterator position, T&& value)
		{
			auto* current_position = position.current;
			this->my_size++;
			return iterator(current_position->previous = current_position->previous->next =
				new node{ std::move(value), current_position->previous, current_position });

		}
		/**
		 * Isolates and erases a node at a given position.
		 *
		 * @param position is the node to be erased.
		 */
		iterator erase(iterator position)
		{
			auto* current_position = position.current;
			iterator value(current_position->next);
			current_position->previous->next = current_position->next;
			current_position->next->previous = current_position->previous;
			// Now I have isolated current position
			delete current_position;
			this->my_size--;
			return value;
		}
		/**
		 * Isolates and erases a range of nodes.
		 *
		 * @param from is the starting position to be erased.
		 * @param to is the ending position to be erased.
		 */
		iterator erase(iterator from, iterator to)
		{
			for (auto position = from; position != to;) {
				position = erase(position);
			}

			return to;
		}
		/**
		 * Locates search key and moves it to the front of list.
		 *
		 * @param key is the value to search the list for.
		 */
		bool find(T key)
		{
			for (auto position = begin(); position != end(); position++) {					// O(n) due to search n times.
				if (*position == key)
				{
					push_front(*position);													// Constant time insertion operation.
					erase(position);														// Constant time erasure operation.
					return true;
				} // else, key is already at the begining of the list. do_nothing();
			}
			return false;
		}																					// Method has an overall O(n) run-time.

		friend std::ostream& operator<<(std::ostream& out, const linked_list& list)
		{
			if (list.empty()) {
				out << "Empty list";
			}
			else {
				for (auto& value : list) {
					out << value << " ";
				}
			}

			return out;
		}

	private:
		/**
		 * The current number of nodes in the list.
		 */
		int my_size;
		/**
		 * Pointer to begining of list.
		 */
		node* head;
		/**
		 * Pointer to tail of the list.
		 */
		node* tail;

		/**
		* Initialization of list.
		*/
		void init()
		{
			this->my_size = 0;
			this->head = new node;
			this->tail = new node;
			this->head->next = this->tail;
			this->tail->previous = this->head;
		}
	};

}
#endif
