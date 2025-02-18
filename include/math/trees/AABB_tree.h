/**
 * @file AABB_tree.h
 * @author Artem
 * @brief AABB дерево дл€ поиска пересекающихс€ подобластей
 * @version 0.1
 * @date 2025-02-18
 * 
 */
#ifndef AABB_TREE_H
#define AABB_TREE_H
#include <vector>
#include <list>

#if 1 /// \todo: #ifndef EIGEN_ACTIVE
template <typename _type = double> class Vector3
{
public:
	Vector3(const _type x = 0, const _type y = 0, const _type z = 0) : _data{ x,y,z } {}
	Vector3(const Vector3& x) : _data{ x[0],x[1],x[2] } {}

	_type operator[](const size_t i) const { return _data[i]; }
	_type& operator[](const size_t i) { return _data[i]; }

	Vector3& operator=(const Vector3& a) {
		_data[0] = a[0];
		_data[1] = a[1];
		_data[2] = a[2];
		return *this;
	}	
	void operator*(const _type a) {
		_data[0] *= a[0];
		_data[1] *= a[1];
		_data[2] *= a[2];		
	}
private:
	_type _data[3];
};
template <typename _type = double>
Vector3<_type> operator*(const _type a, const Vector3<_type>& b) {
	return Vector3(a * b[0], a * b[1], a * b[2]);
}
template <typename _type = double>
Vector3<_type> operator+(const Vector3<_type>& a, const Vector3<_type>& b) {	
	return Vector3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;
#else
#include <Eigen\Dense>
typedef Eigen::Vector3d Vector3;
typedef Eigen::Vector3i Vector3i;
#endif

class AABB_tree
{
	static constexpr int _spatial_scale = 3;	///< размерность пространства
public:

	//! @brief  ќкаймл€ющие объемы	 	 
	class Box
	{
	public:
		Box(const int id = 0, const Vector3d& min= Vector3d(0,0,0), const Vector3d& max= Vector3d(0, 0, 0)) : _id(id), _min(min), _max(max){}		
		Box& operator=(const Box& a)
		{
			_id = a._id; _min = a._min; _max = a._max;
			return *this;
		}
	private:
		friend class AABB_tree;
		int _id;			///< номер €чейки
		Vector3d _min;	///< левый нижний угол пр€моугольника
		Vector3d _max;	///< правый верхний угол пр€моугольника
	};
	

	/**
	 * @brief Construct a new aabb tree object
	 * 
	 * @param max_width 
	 * @param min_cnt минимальный размер листа (кол-ва €чеек)
	 */
	AABB_tree(const Vector3i& min_cnt = Vector3i(8, 8, 8)) :
		left(nullptr),
		right(nullptr),		
		NextDim(nullptr),
		_max_elem(Vector3d(0,0,0)),
		_min_count(min_cnt)		
	{
	}

	
	~AABB_tree()
	{
		if(left) delete left;
		if(right) delete right;
		if (NextDim) delete NextDim;		
	}
	
		
	/**
	 * @brief окаймл€ющие объемы дл€ каждой €чеки сетки
	 * 
	 * @param begin начало сетки
	 * @param end конец сетки
	 */
	void Init(const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end);

	/**
	 * @brief поиск пересекающихс€ объемов по дереву
	 * 
	 * @param A контрольный элемент	 
	 * @param ret возвращает список массивов с номерами €чеек пересечени€ с элементом A
	 */
	int Find(const Box& A, std::list<const std::vector<int>*>& ret) const;

private:

	AABB_tree(const Vector3i& min_cnt, const Vector3d& max_width) :
		left(nullptr),
		right(nullptr),
		NextDim(nullptr),
		_max_elem(max_width),
		_min_count(min_cnt)
	{
	}
	
	/**
	 * @brief —ортировка по максимальной границе (правой)
	 * 
	 * @param begin 
	 * @param end 
	 * @param idx 
	 */
	void sort_domain(const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end, int idx);

	/**
	 * @brief 
	 * 
	 * @param A 
	 * @param spatial_level номер пространсвеной координаты
	 * @param ret 
	 */
	int find_next(const Box& A, int spatial_level, std::list<const std::vector<int>*>& ret) const;

	/**
	 * @brief 
	 * 
	 * @param begin 
	 * @param end 
	 * @param spatial_level  номер пространсвеной координаты
	 */
	void add_next_lvl(const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end, int  spatial_level);			
	
private:	
	AABB_tree* left;		///< лист слева
	AABB_tree* right;		///< лист справа
	AABB_tree* NextDim;		///< переход на следующее пространсвенное направление

	Box box;				///< окаймл€ющие объем текузего листа
	Vector3d _max_elem;		///< самый большой элемент на сетке по каждому и простравенных направлений (служит нижней границей дл€ размера листа дерева)
	Vector3i _min_count;	///< минимальное кол-во элементов в листе дерева
	
	std::vector<int> ids;	///< номер €чеек в листе
};

#endif //AABB_TREE_H