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

	template <typename type1, typename type2, typename maps> class DoubleTensor;
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
	int				cfg;
	type*			vec;
	Vector<maps>	dim;
private:
	decltype(auto) typed_allocate(maps v) { return size_t(0); }
	decltype(auto) allocate() noexcept
	{
		if (cfg & eConfig::cfg_init)
			return;

		auto ds = dim.size();
		if (ds)
		{
			if (!vec)
			{
				if (ds > 1)
				{
					static_cast<type**&>(vec) = new type*[ds];
					for (size_t d = 0; d < ds; ++d)
						static_cast<type**&>(vec)[d] = new type[typed_allocate(dim[d])];
				}
				else vec = new type[typed_allocate(dim[0])];
			}
		}
		else this->free();
		cfg |= eConfig::cfg_init;
	}
	decltype(auto) allocate(size_t al) noexcept
	{
		this->dim.allocate(al);
	}
public:
	decltype(auto) ResetPointer() noexcept
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
		cfg &= ~eConfig::cfg_init;
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
//	еблан блять тупой нахуй, в insert ты используешь свойство push_back, а не insertа - измени!
	decltype(auto) insert(size_t place, type* v, maps map) noexcept
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
	decltype(auto) insert(size_t place, type* v, Vector<maps>& map) noexcept
	{
		if (v && map.size())
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
	decltype(auto) insert(size_t place, PseudoTensor<type, maps>& t) noexcept
	{
		if (t.not_empty())
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
	decltype(auto) insert(size_t place, PseudoTensor<type, maps>* t) noexcept
	{
		if (t->not_empty())
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
					memcpy(static_cast<type**&>(v->vec)[i], static_cast<type**&>(vec)[i], typed_allocate(dim[i]) * sizeof(type));
			else memcpy(v->vec, vec, typed_allocate(dim[0]) * sizeof(type));
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
	decltype(auto) not_empty() noexcept
	{
		return vec && dim.size();
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
	decltype(auto) point_init() noexcept // call after using 1) reserve 2) filling dim 
	{
		allocate();
	}
	decltype(auto) rate(double val) noexcept
	{
		return double&(dim.mul_alloc = val);
	}
	decltype(auto) rate() noexcept
	{
		return double&(dim.mul_alloc);
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

	decltype(auto) begin() noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return iterator(vec);
		else return iterator(reinterpret_cast<type*>(&vec));
	}
	decltype(auto) end() noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return iterator(vec + sz);
		else return iterator(reinterpret_cast<type*>(&vec) + sz);
	}
	decltype(auto) cbegin() const noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return const_iterator(vec);
		else return const_iterator(reinterpret_cast<type*>(&vec));
	}
	decltype(auto) cend() const noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return const_iterator(vec + sz);
		else return const_iterator(reinterpret_cast<type*>(&vec) + sz);
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
	decltype(auto) operator+=(const PseudoTensor<type, maps> v) const noexcept
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
	decltype(auto) typed_allocate(WXYZ v)
	{
		return v.Multiply() /*+ (PseudoTensor<type, WXYZ>::cfg_bias && PseudoTensor<type, WXYZ>::cfg_gpus) * (something X values numerically equal outputs size vector)*/;
	}
public:
	decltype(auto) DotProduct(PseudoTensor<type, MapFilter>&& in, PseudoTensor<type, WXYZ>&& out)
	{
		// bruhed
	}
	decltype(auto) CrossProduct(PseudoTensor<type, MapFilter>&& in, PseudoTensor<type, WXYZ>&& out)
	{
		// bruhed
	}
	decltype(auto) Convolution(PseudoTensor<type, MapFilter>&& in, PseudoTensor<type, WXYZ>&& out)
	{
		// bruhed
	}
	decltype(auto) CrossCorrelation(PseudoTensor<type, MapFilter>&& in, PseudoTensor<type, WXYZ>&& out)
	{
		// bruhed
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
	decltype(auto) typed_allocate(MapFilter v)
	{
		return v.mask_w * v.mask_x * v.mask_y * v.mask_z + PseudoTensor<type, MapFilter>::cfg_bias;
	}
public:

};

template <typename type1, typename type2, typename maps>
class  UltimaAPI::DoubleTensor : public PseudoTensor<type1, maps>
{
	// don't use in current time begin-end for
protected:
	type2* vec2;
private:
	decltype(auto) typed_allocate(maps v) { return size_t(0); }
	decltype(auto) allocate() noexcept
	{
		if (this->cfg & PseudoTensor<type1, maps>::eConfig::cfg_init)
			return;

		auto ds = this->dim.size();
		if (ds)
		{
			// Ideally, only a single pointer should be checked here, but I'm not sure that the pointer will not be deleted / nullified at runtime
			if (!this->vec && !this->vec2) // WARNING This is a slow way, but it is the most accurate in checking. 
			{
				if (ds > 1)
				{
					static_cast<type1**&>(this->vec) =  new type1*[ds];
					static_cast<type1**&>(this->vec2) = new type2*[ds];
					for (size_t d = 0; d < ds; ++d)
					{
						size_t al = typed_allocate(this->dim[d]);
						static_cast<type1**&>(this->vec)[d] =  new type1[al];
						static_cast<type2**&>(this->vec2)[d] = new type2[al];
					}
				}
				else
				{
					size_t al = typed_allocate(this->dim[0]);
					this->vec =  new type1[al];
					this->vec2 = new type2[al];
				}
			}
		}
		else this->free();
		this->cfg |= PseudoTensor<type1, maps>::eConfig::cfg_init;
	}
public:
	decltype(auto) ResetPointer() noexcept
	{
		auto size = this->dim.size();
		if (!this->vec && !this->vec2)
		{
			if (size > 1)
				for (auto i = 0; i < size; i++)
				{
					delete[](reinterpret_cast<void**&>(this->vec)[i]);
					delete[](reinterpret_cast<void**&>(this->vec2)[i]);
				}
			delete[] this->vec;
			delete[] this->vec2;
			this->vec = nullptr;
			this->vec2 = nullptr;
		}
		this->cfg &= ~PseudoTensor<type1, maps>::eConfig::cfg_init;
	}
//	еблан блять тупой нахуй, в insert ты используешь свойство push_back, а не insertа - измени!
	decltype(auto) push_back(DoubleTensor<type1, type2, maps>* t) noexcept
	{
		if (t->vec && t->vec2)
		{
			if (this->vec && this->vec2)
			{
				auto  al = this->dim.alloc_step();
				auto  old_size = this->dim.size();
				auto  push_size = t->dim.size();
				void* ptr1 = new type1*[al];
				void* ptr2 = new type2*[al];
				if (old_size > 1)
				{
					memcpy(ptr1, this->vec,  old_size * sizeof(type1*));
					memcpy(ptr2, this->vec2, old_size * sizeof(type2*));
				}
				else
				{
					ptr1[0] = this->vec;
					ptr2[0] = this->vec2;
				}
				if (push_size > 1)
				{
					memcpy(&ptr1[old_size], t->vec,  push_size * sizeof(type1*));
					memcpy(&ptr2[old_size], t->vec2, push_size * sizeof(type2*));
				}
				else
				{
					ptr1[old_size] = t->vec;
					ptr2[old_size] = t->vec2;
				}
				delete[] this->vec;
				delete[] this->vec2;
				this->vec =  ptr1;
				this->vec2 = ptr2;
			}
			else
			{
				this->vec =  t->vec;
				this->vec2 = t->vec2;
			}
			this->dim += t->dim;
		}
	}
	decltype(auto) push_back(type1* v1, type2* v2, maps v) noexcept
	{
		if (this->vec && this->vec2)
		{
			auto  al = this->dim.alloc_step();
			auto  old_size = this->dim.size();
			void* ptr1 = new type1*[al];
			void* ptr2 = new type2*[al];
			if (old_size > 1)
			{
				memcpy(ptr1, this->vec,  old_size * sizeof(type1*));
				memcpy(ptr2, this->vec2, old_size * sizeof(type2*));
			}
			else
			{
				ptr1[0] = this->vec;
				ptr2[0] = this->vec2;
			}
			ptr1[old_size] = v1;
			ptr2[old_size] = v2;
			delete[] this->vec;
			delete[] this->vec2;
			this->vec =  ptr1;
			this->vec2 = ptr2;
		}
		else
		{
			this->vec =  v1;
			this->vec2 = v2;
		}
		this->dim.push_back(v);
	}
	decltype(auto) pop_back() noexcept
	{
		auto ds = this->dim.size();
		if (ds)
		{
			if (ds > 1)
			{
				delete[] static_cast<type1**&>(this->vec)[ds - 1];
				delete[] static_cast<type2**&>(this->vec2)[ds - 1];
			}
			else
			{
				delete[] this->vec;
				delete[] this->vec2;
			}
		}
		this->dim.pop_back();
	}
	decltype(auto) insert(size_t place, type1* v1, type2* v2, maps map) noexcept
	{
		/*
		if (v1 && v2)
		{
			if (this->vec && this->vec2)
			{
				auto  al = dim.alloc_step();
				auto  old_size = dim.size();
				void* ptr1 = new type1*[al];
				void* ptr2 = new type2*[al];
				if (old_size > 1)
				{
					memcpy(ptr1, this->vec,  old_size * sizeof(type1*));
					memcpy(ptr2, this->vec2, old_size * sizeof(type2*));
				}
				else
				{
					ptr1[0] = this->vec;
					ptr2[0] = this->vec2;
				}
				ptr1[old_size] = v1;
				ptr2[old_size] = v2;
				delete[] this->vec;
				delete[] this->vec2;
				this->vec =  ptr1;
				this->vec2 = ptr2;
			}
			else
			{
				this->vec =  v1;
				this->vec2 = v2;
			}
			dim.push_back(map);
		}
		*/
	}
	decltype(auto) insert(size_t place, type1* v1, type2* v2, Vector<maps>& map) noexcept
	{
		
	}
	decltype(auto) insert(size_t place, DoubleTensor<type1, type2, maps>& t) noexcept
	{
		
	}
	decltype(auto) insert(size_t place, DoubleTensor<type1, type2, maps>* t) noexcept
	{
		
	}
	decltype(auto) size() noexcept
	{
		return this->dim.size();
	}
	decltype(auto) copy(DoubleTensor<type1, type2, maps>* v) noexcept
	{
		v->allocate(this->dim.allocated);
		this->dim.copy(v->dim);
		if (this->dim.used)
		{
			v->allocate();
			if (this->dim.used > 1)
				for (size_t i = 0; i < this->dim.used; ++i)
				{
					memcpy(static_cast<type1**&>(v->vec)[i],  static_cast<type1**&>(this->vec)[i],  typed_allocate(this->dim[i]) * sizeof(type1));
					memcpy(static_cast<type2**&>(v->vec2)[i], static_cast<type2**&>(this->vec2)[i], typed_allocate(this->dim[i]) * sizeof(type2));
				}
			else
			{
				memcpy(v->vec, this->vec,   typed_allocate(this->dim[0]) * sizeof(type1));
				memcpy(v->vec2, this->vec2, typed_allocate(this->dim[0]) * sizeof(type2));
			}
		}
	}
	decltype(auto) back() noexcept
	{
		return *this->vec.last;
	}
	decltype(auto) capacity() noexcept
	{
		return this->vec.capacity();
	}
	decltype(auto) data() noexcept
	{
		return this->vec;
	}
	decltype(auto) swap(DoubleTensor<type1, type2, maps>& v) noexcept
	{
		std::swap(*this, v);
	}
	decltype(auto) not_empty() noexcept
	{
		return this->vec && this->dim.size();
	}
	decltype(auto) resize(size_t sz) noexcept
	{
		this->dim.resize(sz);
	}
	decltype(auto) free() noexcept
	{
		ResetPointer();
		this->dim.free();
	}
	decltype(auto) reserve(size_t sz) noexcept
	{
		allocate(sz);
	}
	decltype(auto) point_init() noexcept // call after using 1) reserve 2) filling dim 
	{
		allocate();
	}
	decltype(auto) rate(double val) noexcept
	{
		return double& (this->dim.mul_alloc = val);
	}
	decltype(auto) rate() noexcept
	{
		return double& (this->dim.mul_alloc);
	}
	decltype(auto) sizeof_type1() noexcept
	{
		return sizeof(type1);
	}
	decltype(auto) sizeof_type2() noexcept
	{
		return sizeof(type2);
	}
	decltype(auto) sizeof_maps() noexcept
	{
		return sizeof(maps);
	}
	decltype(auto) shrink_to_fit() noexcept
	{
		this->dim.shrink_to_fit();
		auto old_size = this->dim.size();
		if (old_size > 1)
		{
			void* ptr = new type1 * [old_size];
			memcpy(ptr, this->vec, old_size * sizeof(type1*));
			delete[] this->vec;
			this->vec = ptr;
		}
	}

	/*
	decltype(auto) begin() noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return iterator(vec);
		else return iterator(reinterpret_cast<type*>(&vec));
	}
	decltype(auto) end() noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return iterator(vec + sz);
		else return iterator(reinterpret_cast<type*>(&vec) + sz);
	}
	decltype(auto) cbegin() const noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return const_iterator(vec);
		else return const_iterator(reinterpret_cast<type*>(&vec));
	}
	decltype(auto) cend() const noexcept
	{
		auto sz = dim.size();
		if (sz > 1)
			return const_iterator(vec + sz);
		else return const_iterator(reinterpret_cast<type*>(&vec) + sz);
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
	*/

	decltype(auto) operator~() noexcept
	{
		free();
	}
	decltype(auto) operator^=(DoubleTensor<type1, type2, maps>& v) noexcept
	{
		swap(v);
	}
	decltype(auto) operator+=(DoubleTensor<type1, type2, maps> v) noexcept
	{
		insert(this->dim.size(), v);
	}
	decltype(auto) operator+=(DoubleTensor<type1, type2, maps>& v) noexcept
	{
		insert(this->dim.size(), v);
	}
	decltype(auto) operator+=(DoubleTensor<type1, type2, maps>&& v) noexcept
	{
		insert(this->dim.size(), v);
	}
	decltype(auto) operator+=(const DoubleTensor<type1, type2, maps> v) const noexcept
	{
		insert(this->dim.size(), v);
	}
	decltype(auto) operator+=(const DoubleTensor<type1, type2, maps>& v) const noexcept
	{
		insert(this->dim.size(), v);
	}
	decltype(auto) operator+=(const DoubleTensor<type1, type2, maps>&& v) const noexcept
	{
		insert(this->dim.size(), v);
	}
	decltype(auto) operator[](size_t i) noexcept
	{
		this->dim[i];
		return std::pair<type1, type2>(this->vec[i], this->vec2[i]);
	}

	DoubleTensor() noexcept
	{
		this->vec = nullptr;
		this->vec2 = nullptr;
	}
	DoubleTensor(size_t sz) noexcept
	{
		this->vec = nullptr;
		this->vec2 = nullptr;
		allocate(sz);
	}
	DoubleTensor(size_t sz, type1* r1, type2* r2) noexcept
	{
		this->vec = nullptr;
		this->vec2 = nullptr;
		insert(0, r1, r2, sz);
	}
	DoubleTensor(DoubleTensor<type1, type2, maps>& v) noexcept
	{
		v.copy(this);
	}

	~DoubleTensor() noexcept
	{
		free();
	}
};