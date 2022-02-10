#include <vector>
#include <unordered_map>
template<typename T>
class matrix{
	template<typename F>
	friend matrix<F> operator*(const matrix<F>& lhs,const matrix<F>& rhs);
	public:
		matrix(int rows=0,int cols=0):data(rows,std::vector<T>(cols)){}
	        matrix(const matrix& other):data(other.data){}	
		matrix(matrix&& other):data(std::move(other.data)){}
		//resize matrix
		void resize(int rows,int cols){
			data.resize(rows,std::vector<T>(cols));	
			return;
		}

		// set matrix element
		bool set(int r,int c,T val){
			int rows = data.size();
			if(rows==0) return false;
			int cols = data[0].size();
			if(r<0||c<0||r>=rows||c>=cols) return false;
			data[r][c] = val;
			return true;	
		}	

		// get matrix element
		bool get(int r,int c,T& val){
			int rows = data.size();
			if(rows==0) return false;
			int cols = data[0].size();
			if(r<0||c<0||r>=rows||c>=cols) return false;
			val = data[r][c];
			return true;
		}
		
		matrix operator*=(const matrix& rhs)
		{
			//if matrix size doesnt match, return
			if(data.empty()||data[0].empty()) return {};
			if(data[0].size()!=rhs.size()) return {};

			int new_rows = data.size(),new_cols = rhs[0].size();
			matrix res(new_rows,new_cols);
			for(int i=0;i<new_rows;i++)
			{
				for(int j=0;j<new_cols;j++)	
				{
					for(int k=0;k<data[0].size();k++)
					{
						res.data[i][j]+=data[i][k]*rhs.data[k][j];	
					}
				}
			}
			return res;
		}
		matrix transpose() const
		{
			if(data.empty()||data[0].empty()) return {};
			int rows = data.size(),cols = data[0].size();
			matrix res(cols,rows);
			for(int i=0;i<data.size();i++)
			{
				for(int j=0;j<data[0].size();j++)	
				{
					res.data[j][i] = data[i][j];	
				}
			}
			return res;
		}

	private:
		std::vector<std::vector<T>> data;

};
template<typename F>
matrix<F> operator*(const matrix<F>& lhs, const matrix<F>& rhs){
	matrix<F> res(lhs);		
	res *= rhs;
	return rhs;
}


class sparse_matrix{
	friend sparse_matrix operator+(const sparse_matrix& lhs,const sparse_matrix& rhs);
	public:
		sparse_matrix(int r,int c):rows_num(r),cols_num(c){}
		bool set(int r,int c,int val)
		{
			if(r>=rows_num || c>=cols_num)
				return false;
			m[r][c] = val;	
			return true;
		}
		bool get(int r,int c,int& val)
		{
			if(r>=rows_num || c>=cols_num)
				return false;
			if(m.count(r)==0 || m[r].count(c)==0)
				return false;
			val = m[r][c];
			return true;
		}
		sparse_matrix& operator+=(const sparse_matrix& other){
			for(auto& r: other.m)
			{
				int row = r.first;
				for(auto& c:r.second)
				{
					int col = c.first, val = c.second;	
					m[row][col] += val;
					if(m[row][col]==0)
						m[row].erase(col);
				}
				if(m[row].empty())
					m.erase(row);
			}
			return *this;
		}

	private:
		int rows_num;
		int cols_num;
		std::unordered_map<int,std::unordered_map<int,int>> m; 
};


sparse_matrix operator+(const sparse_matrix& lhs,const sparse_matrix& rhs)
{
	sparse_matrix sp_matrix(lhs);
	sp_matrix += rhs;
	return sp_matrix;
}



