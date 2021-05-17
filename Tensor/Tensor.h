#pragma once

#include <memory.h>
#include <initializer_list>

#include "../Vector/Vector_old.h"
#include "../Memory/util.h"

namespace UltimaAPI
{
	struct WXYZ;
	struct MapFilter;

	template <typename type, typename maps>	class PseudoTensor;
	template <typename type>				class Tensor;
	template <typename type>				class TensorFilter;
}

struct UltimaAPI::WXYZ
{
	size_t wxyz[4] = { 0 };
	__inline size_t* operator()() { return wxyz; }
	__inline size_t& operator[](int i) { return wxyz[i]; }

	__inline WXYZ() { }
	__inline WXYZ(size_t z, size_t y = 1, size_t x = 1, size_t w = 1)
	{
		wxyz[0] = w; wxyz[1] = x; wxyz[2] = y; wxyz[3] = z;
	}

	__inline size_t Multiply() { return wxyz[0] * wxyz[1] * wxyz[2] * wxyz[3]; }

	decltype(auto) Maxed(WXYZ& i)
	{
		if (wxyz[0] < i[0]) wxyz[0] = i[0];
		if (wxyz[1] < i[1]) wxyz[1] = i[1];
		if (wxyz[2] < i[2]) wxyz[2] = i[2];
		if (wxyz[3] < i[3]) wxyz[3] = i[3];
		return *this;
	}
	decltype(auto) Maxed(size_t z, size_t y, size_t x, size_t w)
	{
		if (wxyz[0] < w) wxyz[0] = w;
		if (wxyz[1] < x) wxyz[1] = x;
		if (wxyz[2] < y) wxyz[2] = y;
		if (wxyz[3] < z) wxyz[3] = z;
		return *this;
	}

	decltype(auto) operator+=(const WXYZ& a)
	{
		this->wxyz[0] += a.wxyz[0];
		this->wxyz[1] += a.wxyz[1];
		this->wxyz[2] += a.wxyz[2];
		this->wxyz[3] += a.wxyz[3];
		return *this;
	}

	__inline WXYZ operator+(WXYZ a)
	{
		return WXYZ(this->wxyz[1] + a.wxyz[1], this->wxyz[2] + a.wxyz[2], this->wxyz[3] + a.wxyz[3], this->wxyz[0] + a.wxyz[0]);
	}

	__inline bool       operator<(WXYZ a)
	{
		return this->wxyz[0] < a[0] && this->wxyz[1] < a[1] && this->wxyz[2] < a[2] && this->wxyz[3] < a[3];
	}
	__inline bool       operator>(WXYZ a)
	{
		return this->wxyz[0] > a[0] && this->wxyz[1] > a[1] && this->wxyz[2] > a[2] && this->wxyz[3] > a[3];
	}
	__inline bool       operator==(WXYZ a)
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline bool       operator!=(WXYZ a)
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline bool       operator==(WXYZ& a)
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline bool       operator!=(WXYZ& a)
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline const bool operator==(WXYZ a) const
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline const bool operator!=(WXYZ a) const
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline const bool operator==(WXYZ& a) const
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
	__inline const bool operator!=(WXYZ& a) const
	{
		return wxyz[0] == a[0] && wxyz[1] == a[1] && wxyz[2] == a[2] && wxyz[3] == a[3];
	}
};

struct UltimaAPI::MapFilter
{
	// mask_(WXYZ),			This is the NUMBER of elements for each dimension.
	// padding_(WXYZ),		This is the number of cells for padding. [for a 3x3 mask with a rarity of 0, so that the convolution image needs to be set to 1 in padding]
	// strider_step_(WXYZ),	This is responsible for what step the filter will take on the input array.
	// sparsity_(WXYZ)		[in developing] This is responsible for how tightly the filter cells will be located on the input array. 
	size_t mask_w, mask_x, mask_y, mask_z;
	int    padding_w, padding_x, padding_y, padding_z;
	size_t strider_step_w, strider_step_x, strider_step_y, strider_step_z;
	size_t sparsity_w, sparsity_x, sparsity_y, sparsity_z;
};

template <typename type, typename maps>
class  UltimaAPI::PseudoTensor
{
	friend class Tensor<type>;
	friend class TensorFilter<type>;
public:
	using iterator = BasicIterator<type>;
	using const_iterator = BasicIterator<const type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	enum eConfig
	{
		cfg_null = 0,

		cfg_init = 1 << 0,
		cfg_bias = 1 << 1,
		cfg_gpus = 1 << 2
	};
protected:
	eConfig			cfg;
	type*			vec;
	Vector<maps>	dim;
private:
	decltype(auto) allocate() { }
	decltype(auto) allocate(size_t) { }
public:
	decltype(auto) ResetPointer()
	{
		auto size = dim.size();
		if (vec)
		{
			if (size > 1)
				for (auto i = 0; i < size; i++)
					delete[](reinterpret_cast<void**&>(vec)[i]);
			delete[] vec;
			vec = nullptr;
		}
	}

	decltype(auto) push_back(PseudoTensor<type, maps>* t) noexcept
	{
		if (t->vec)
		{
			if (vec)
			{
				auto  old_size = dim.size();
				auto  push_size = t->dim.size();
				void* ptr = new type*[dim.alloc_step()];
				if (old_size > 1)
					memcpy(ptr, vec, old_size * sizeof(type*));
				else ptr[0] = vec;
				if (push_size > 1)
					memcpy(&ptr[old_size], t->vec, push_size * sizeof(type*));
				else ptr[old_size] = t->vec;
				delete[] vec;
				vec = ptr;
			}
			else vec = t->vec;
			dim += t->dim;
		}
	}
	decltype(auto) push_back(type* val, maps v) noexcept
	{
		if (vec)
		{
			auto  old_size = dim.size();
			void* ptr = new type*[dim.alloc_step()];
			if (old_size > 1)
				memcpy(ptr, vec, old_size * sizeof(type*));
			else ptr[0] = vec;
			ptr[old_size] = val;
			delete[] vec;
			vec = ptr;
		}
		else vec = val;
		dim.push_back(v);
	}
	decltype(auto) pop_back() noexcept
	{
		auto ds = dim.size();
		if (ds)
		{
			if (ds > 1)
				delete[] static_cast<type**&>(vec)[ds - 1];
			else delete[] vec;
		}
		dim.pop_back();
	}
	decltype(auto) insert(size_t place, type* v, maps map)
	{
		if (v)
		{
			if (vec)
			{
				auto  old_size = dim.size();
				void* ptr = new type*[dim.alloc_step()];
				if (old_size > 1)
					memcpy(ptr, vec, old_size * sizeof(type*));
				else ptr[0] = vec;
				ptr[old_size] = v;
				delete[] vec;
				vec = ptr;
			}
			else vec = v;
			dim.push_back(map);
		}
	}
	decltype(auto) insert(size_t place, type* v, Vector<maps>& map)
	{
		if (v)
		{
			if (vec)
			{
				auto  old_size = dim.size();
				auto  push_size = map.size();
				void* ptr = new type*[dim.alloc_step()];
				if (old_size > 1)
					memcpy(ptr, vec, old_size * sizeof(type*));
				else ptr[0] = vec;
				if (push_size > 1)
					memcpy(&ptr[old_size], v, push_size * sizeof(type*));
				else ptr[old_size] = v;
				delete[] vec;
				vec = ptr;
			}
			else vec = v;
			dim += map;
		}
	}
	decltype(auto) insert(size_t place, PseudoTensor<type, maps>& t)
	{
		if (t.vec)
		{
			if (vec)
			{
				auto  old_size = dim.size();
				auto  push_size = t.dim.size();
				void* ptr = new type*[dim.alloc_step()];
				if (old_size > 1)
					memcpy(ptr, vec, old_size * sizeof(type*));
				else ptr[0] = vec;
				if (push_size > 1)
					memcpy(&ptr[old_size], t.vec, push_size * sizeof(type*));
				else ptr[old_size] = t.vec;
				delete[] vec;
				vec = ptr;
			}
			else vec = t.vec;
			dim += t.dim;
		}
	}
	decltype(auto) insert(size_t place, PseudoTensor<type, maps>* t)
	{
		if (t->vec)
		{
			if (vec)
			{
				auto  old_size = dim.size();
				auto  push_size = t->dim.size();
				void* ptr = new type * [dim.alloc_step()];
				if (old_size > 1)
					memcpy(ptr, vec, old_size * sizeof(type*));
				else ptr[0] = vec;
				if (push_size > 1)
					memcpy(&ptr[old_size], t->vec, push_size * sizeof(type*));
				else ptr[old_size] = t->vec;
				delete[] vec;
				vec = ptr;
			}
			else vec = t->vec;
			dim += t->dim;
		}
	}
	decltype(auto) size() noexcept
	{
		return dim.size();
	}
	decltype(auto) copy(PseudoTensor<type, maps>* v) noexcept
	{
		v->allocate(dim.allocated);
		dim.copy(v->dim);
		if (dim.used)
		{
			v->allocate();
			if (dim.used > 1)
				for (size_t i = 0; i < dim.used; ++i)
					memcpy(static_cast<type**&>(v->vec)[i], static_cast<type**&>(vec)[i], dim[i].Multiply() * sizeof(type));
			else memcpy(v->vec, vec, dim[0].Multiply() * sizeof(type));
		}
	}
	decltype(auto) back() noexcept
	{
		return *dim.last;
	}
	decltype(auto) capacity() noexcept
	{
		return dim.capacity();
	}
	decltype(auto) data() noexcept
	{
		return vec;
	}
	decltype(auto) swap(PseudoTensor<type, maps>& v) noexcept
	{
		std::swap(*this, v);
	}
	decltype(auto) empty() noexcept
	{
		return dim.used == 0;
	}
	decltype(auto) resize(size_t sz) noexcept
	{
		dim.resize(sz);
	}
	decltype(auto) free() noexcept
	{
		ResetPointer();
		dim.free();
	}
	decltype(auto) reserve(size_t sz) noexcept
	{
		allocate(sz);
	}
	decltype(auto) rate(double val) noexcept
	{
		return double& (dim.mul_alloc = val);
	}
	decltype(auto) rate() noexcept
	{
		return double& (dim.mul_alloc);
	}
	decltype(auto) sizeof_type() noexcept
	{
		return sizeof(type);
	}
	decltype(auto) sizeof_maps() noexcept
	{
		return sizeof(maps);
	}
	decltype(auto) shrink_to_fit() noexcept
	{
		dim.shrink_to_fit();
		auto old_size = dim.size();
		if (old_size > 1)
		{
			void* ptr = new type*[old_size];
			memcpy(ptr, vec, old_size * sizeof(type*));
			delete[] vec;
			vec = ptr;
		}
	}

	decltype(auto) DotProduct(PseudoTensor<type, maps>* t1)
	{
		return (type)0;
}
	decltype(auto) CrossProduct(PseudoTensor<type, maps>* t1)
	{
		return PseudoTensor<type, maps>();
}
	decltype(auto) Convolution(PseudoTensor<type, maps>* t1)
	{
		return PseudoTensor<type, maps>();
}
	decltype(auto) CrossCorrelation(PseudoTensor<type, maps>* t1)
	{
		return PseudoTensor<type, maps>();
	}

	decltype(auto) begin() noexcept
	{
		return iterator(dim.start);
	}
	decltype(auto) end() noexcept
	{
		return iterator(dim.start + dim.used);
	}
	decltype(auto) cbegin() const noexcept
	{
		return const_iterator(dim.start);
	}
	decltype(auto) cend() const noexcept
	{
		return const_iterator(dim.start + dim.used);
	}
	decltype(auto) rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	decltype(auto) rend() noexcept
	{
		return reverse_iterator(begin());
	}
	decltype(auto) crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}
	decltype(auto) crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	decltype(auto) operator~() noexcept
	{
		free();
	}
	decltype(auto) operator^=(PseudoTensor<type, maps>& v) noexcept
	{
		swap(v);
	}
	decltype(auto) operator+=(PseudoTensor<type, maps> v) noexcept
	{
		insert(dim.size(), v);
	}
	decltype(auto) operator+=(PseudoTensor<type, maps>& v) noexcept
	{
		insert(dim.size(), v);
	}
	decltype(auto) operator+=(PseudoTensor<type, maps>&& v) noexcept
	{
		insert(dim.size(), v);
	}
	decltype(auto) operator+=(const PseudoTensor<type, maps> v) const  noexcept
	{
		insert(dim.size(), v);
	}
	decltype(auto) operator+=(const PseudoTensor<type, maps>& v) const noexcept
	{
		insert(dim.size(), v);
	}
	decltype(auto) operator+=(const PseudoTensor<type, maps>&& v) const noexcept
	{
		insert(dim.size(), v);
	}
	decltype(auto) operator[](size_t i) noexcept
	{
		dim[i];
		return vec[i];
	}

	PseudoTensor() noexcept
	{
		vec = nullptr;
	}
	PseudoTensor(size_t sz) noexcept
	{
		vec = nullptr;
		allocate(sz);
	}
	PseudoTensor(size_t sz, type* ray) noexcept
	{
		vec = nullptr;
		insert(0, ray, sz);
	}
	PseudoTensor(PseudoTensor<type, maps>& v) noexcept
	{
		v.copy(this);
	}

	~PseudoTensor() noexcept
	{
		free();
	}
};

template <typename type>
class  UltimaAPI::Tensor : public PseudoTensor<type, WXYZ>
{
private:
	decltype(auto) allocate() noexcept
	{
		auto ds = dim.size();
		if (ds)
		{
			if (!vec)
			{
				if (ds > 1)
				{
					static_cast<type**&>(vec) = new type*[ds];
					for (size_t d = 0; d < ds; ++d)
					{
						auto sz = dim[d].Multiply();
						auto&& v = static_cast<type**&>(vec)[d] = new type[sz];
					//	Memory::memset(v, v + sz, (type)0);  // I'm not sure if this is necessary
					}
				}
				else
				{
					auto sz = dim[0].Multiply();
					vec = new type[sz];
				//	Memory::memset(vec, vec + sz, (type)0);  // I'm not sure if this is necessary
				}
			}
		}
		else free();
	}
	decltype(auto) allocate(size_t al)
	{
		dim.allocate(al);
	}
public:
	decltype(auto) insert(size_t place, type* v, WXYZ map)
	{
		if (map[0] && map[1] && map[2] && map[3])
			__super::insert(place, v, map);
	}
	decltype(auto) insert(size_t place, type* v, Vector<WXYZ>& map)
	{
		if (map.size())
			__super::insert(place, v, map);
	}
	decltype(auto) insert(size_t place, PseudoTensor<type, WXYZ>& t)
	{
		if (t.dim.size())
			__super::insert(place, t);
	}
	decltype(auto) insert(size_t place, PseudoTensor<type, WXYZ>* t)
	{
		if (t->dim.size())
			__super::insert(place, t);
	}
	
	Tensor() noexcept : PseudoTensor<type, WXYZ>() { }
	Tensor(size_t sz) noexcept : PseudoTensor<type, WXYZ>(sz) { }
	Tensor(size_t sz, type* ray) noexcept : PseudoTensor<type, WXYZ>(sz, ray) { }
	Tensor(Tensor<type>& v) noexcept : PseudoTensor<type, WXYZ>(v) { }

	~Tensor() noexcept { }
};

template <typename type>
class  UltimaAPI::TensorFilter : public PseudoTensor<type, MapFilter>
{
	friend class Tensor<type>;
public:
	decltype(auto)  allocate()
	{
		if (cfg & cfg_init)
			return (void)false;

		auto ds = dim.size();
		if (ds)
		{
			if (ds > 1)
			{

			}
			else
			{
				auto f = dim[0];
			//	vec = new 
			}
		}
		else
		{
			// throw
			return (void)false;
		}
	}

	
};
