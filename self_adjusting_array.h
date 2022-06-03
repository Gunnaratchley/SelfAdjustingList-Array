#ifndef SELF_ADJUSTING_ARRAY_H
#define SELF_ADJUSTING_ARRAY_H

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace nwacc {

	/**
	 * Resizable-array implementation of a list.
	 *
	 * This class provides methods to manipulate the size of the array
	 * that is used internally to store the list.
	 *
	 * (This class is roughly equivalent to vector.)
	 *
	 * @author Shane Carroll May
	 * @sub-author Gunnar Atchley
	 */
	template <typename T>
	class array_list {

	public:

		/**
		 * Constructs an empty list with the specified initial capacity.
		 *
		 * @param initial_capacity the initial capacity of the list.
		 */
		explicit array_list(int initial_capacity = 0) :
			my_size{ 0 }, my_capacity{ initial_capacity + k_spare_capacity }
		{
			this->data = new T[this->my_capacity];
		}

		/**
		 * Constructs an array_list from the given list.
		 * This instance will contain a copy of each of the elements in rhs, in the same order.
		 *
		 * @param rhs the list to copy.
		 */
		array_list(const array_list& rhs) :
			my_size{ rhs.my_size }, my_capacity{ rhs.my_capacity },
			data{ nullptr }
		{
			// We are making a copy of one array to another.
			this->data = new T[this->my_capacity];
			for (auto index = 0; index < this->my_size; index++) {
				this->data[index] = rhs.data[index];
			}
		}

		/**
		 * Assigns to this instance the given list.
		 * This will replace its current contents, and modify its size accordingly.
		 *
		 * @param rhs the list to assign to this list.
		 */
		array_list& operator=(const array_list& rhs)
		{
			auto copy = rhs;
			std::swap(*this, copy);
			return *this;
		}

		/**
		 * Destroys this instance and frees any allocated resources.
		 */
		~array_list()
		{
			delete[] this->data;
		}

		/**
		 * Constructs an array_list from the given list.
		 * This instance will contain a copy of each of the elements in rhs, in the same order.
		 *
		 * @param rhs the list to copy.
		 */
		array_list(array_list&& rhs) :
			my_size{ rhs.my_size }, my_capacity{ rhs.my_capacity },
			data{ rhs.data }
		{
			// We have taken everything from rhs. We want
			// to leave rhs in a valid state.
			rhs.data = nullptr;
			rhs.my_capacity = 0;
			rhs.my_size = 0;
		}

		/**
		 * Assigns to this instance the given list.
		 * This will replace its current contents, and modify its size accordingly.
		 *
		 * @param rhs the list to assign to this list.
		 */
		array_list& operator=(array_list&& rhs)
		{
			std::swap(this->my_size, rhs.my_size);
			std::swap(this->my_capacity, rhs.my_capacity);
			std::swap(this->data, rhs.data);
			return *this;
		}

		/**
		 * Returns whether if this instance is empty.
		 *
		 * @return true if this instance is empty, otherwise, false.
		 */
		bool empty() const
		{
			return this->size() == 0;
		}

		/**
		 * Returns the number of elements in this instance.
		 *
		 * @return the current number of elements in this list.
		 */
		int size() const
		{
			return this->my_size;
		}

		/**
		 * Returns the size of the storage space currently allocated for this instance.
		 *
		 * @return the size of the currently allocated storage capacity in this instance.
		 */
		int capacity() const
		{
			return this->my_capacity;
		}

		/**
		 * Returns a reference to the element at position index in the list.
		 *
		 * @param index the index at which to get the value.
		 */
		T& operator[](int index)
		{
			if (index < 0 || index >= this->size()) {
				throw std::out_of_range("Index out of range");
			} // else, index is valid, do_nothing();

			return this->data[index];
		}

		/**
		 * Returns a constant reference to the element at position index in the list.
		 *
		 * @param index the index at which to get the value.
		 */
		const T& operator[](int index) const
		{
			if (index < 0 || index >= this->size()) {
				throw std::out_of_range("Index out of range");
			} // else, index is valid, do_nothing();

			return this->data[index];
		}

		/**
		 * Resizes this instance so that it contains new_size elements.
		 *
		 * @param new_size the new size of this list.
		 */
		void resize(int new_size)
		{
			// Here all we need to check is new size is not less than the capacity.
			if (new_size > this->my_capacity) {
				reserve((new_size * 3) / 2);
			} // else, the capacity is fine, we set the size effectively "deleting" elements in the list, do_nothing();

			this->my_size = new_size;
		}

		/**
		 * Requests that the list capacity be at least enough to contain new_capacity elements.
		 *
		 * @param new_capacity the new list capacity.
		 */
		void reserve(int new_capacity)
		{
			// if the new capacity is less than size, we have nothing to do. 
			if (new_capacity < this->my_size) {
				return;
			} // else, we need to reserve more memory, do_nothing();

			// Allocate more memory for the array. 
			T* new_data = new T[new_capacity];
			// Copy each element from the old array into the new array. 
			for (auto index = 0; index < this->my_size; index++) {
				new_data[index] = std::move(this->data[index]);
			}
			// Change my capacity to the new amount. 
			this->my_capacity = new_capacity;
			// We do this so I do not have to delete data. 
			std::swap(this->data, new_data);
			// After I copy it, I need to delete the newly allocated memory. 
			delete[] new_data;
		}

		/**
		 * Adds a new element at the end of the list, after its current last element.
		 *
		 * @param value the value to add to the list.
		 */
		void push_back(const T& value)
		{
			if (this->my_size == this->my_capacity) {
				// This means I have ran out of room
				// I need a bigger array.
				this->reserve((this->capacity() * 3) / 2);
			} // else, the size is fine, do_nothing();
			this->data[this->my_size++] = value;
		}

		/**
		 * Adds a new element at the end of the list, after its current last element.
		 * This new element is constructed in place.
		 *
		 * @param value the value to add to the list.
		 */

		void emplace_back(T&& value)
		{
			if (this->my_size == this->my_capacity) {
				this->reserve((this->capacity() * 3) / 2);
			} // else, the size is find, do_nothing();
			// Notice here, we can move the rvalue not copy it like in push_back
			this->data[this->my_size++] = std::move(value);
		}

		/**
		 * Removes the last element in the list, this reduces the size of the list by one.
		 */
		void pop_back()
		{
			if (this->empty()) {
				throw std::out_of_range("List is empty");
			} // else, we have elements so remove the last one. 
			--this->my_size;
		}

		/**
		 * Returns constant a reference to the last element in the list.
		 *
		 * @return a constant last element in the list.
		 */
		const T& back() const
		{
			if (this->empty()) {
				throw std::out_of_range("List is empty");
			} // else, we have values, do_nothing();

			return this->data[this->my_size - 1];
		}

		typedef T* iterator;
		typedef const T* const_iterator;

		/**
		 * Returns an iterator pointing to the first element in the list.
		 *
		 * @return an iterator to the beginning of the list.
		 */
		iterator begin()
		{
			return &this->data[0];
		}

		/**
		 * Returns a constant iterator pointing to the first element in the list.
		 *
		 * @return a constant iterator to the first element in the list.
		 */
		const_iterator begin() const
		{
			return &this->data[0];
		}

		/**
		 * Returns an iterator referring to the past-the-end element in the list.
		 *
		 * @return an iterator to the element past the end of the list.
		 */
		iterator end()
		{
			return &this->data[this->size()];
		}

		/**
		 * Returns a constant iterator pointing to the past-the-end element in the list.
		 *
		 * @return a constant iterator to the element past the end of the list.
		 */
		const_iterator end() const
		{
			return &this->data[this->size()];
		}

		/**
		 * A modifier to ensure we do not have a zero (0) capacity in the list.
		 */
		static const int k_spare_capacity = 2;

		/**
		 * Searches array_list for key then moves to front.
		 *
		 * @param key is the value you are searching for.
		 */
		const bool find(const T& key)
		{
			for (auto index = 0; index < my_size; index++) {							// O(n) due to search n times.
				if (key == data[index]) {
					auto new_start = key;
					auto next_value_temp = key;
					for (auto counter = 0; counter <= index; counter++) {				// O(n) counting through n times.
						next_value_temp = data[counter];								
						data[counter] = new_start;										
						new_start = next_value_temp;
					}
					return true;
				} // else, data is not the wanted value. do_nothing();					// Overal run-time of O(n)
			}
			return false;
		}

	private:
		/**
		 * The current number of elements in the list.
		 */
		int my_size;
		/**
		 * The current capacity of the list.
		 */
		int my_capacity;
		/**
		 * A pointer to the backing array.
		 */
		T* data;
	};

}

#endif
