#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

void swap(int &a,int &b){
	int temp = a;
	a = b;
	b = temp;
}
void coutVector(vector<int> a){
	for(auto i=a.begin();i<a.end();++i){
		cout<<*i<<" ";
	}
	cout<<endl;
}
void coutVVector(vector<vector<int>> a){
	for(auto i = a.begin();i!=a.end();++i){
		coutVector(*i);
	}
}
/********************/
//

void backtracking(vector<int>& nums,vector <vector <int>>& res,int level){
	if (level == nums.size()-1){
		res.push_back(nums);
		return ;
	}
	for( int i = level ; i< nums.size(); ++i){
		swap(nums[i],nums[level]);
		backtracking(nums,res,level+1);//可以证明此处发生了交换但是对上层不影响，逻辑之美
		swap(nums[i],nums[level]);
	}
	return;
}


class Solution_1 {//leetcode官方解法，部分理解，但尚未完全证明正确性
public://两分钟就忘记了，如何真正理解？
	//两个问题：1.算法不重复  2.算法不遗漏
	//这两个问题很容易证明正确
	//最大的问题：注意力全部集中在程序实现上，而没有想算法是什么样子的，导致程序很难看懂，因为程序就是算法的表达，因此很难反推算法如何如何，实际上应该先彻底
	//弄明白算法再去思考递归程序的原理。
	//为什么会这样？因为对自己过分自信，总觉得可以直接从程序看懂，然后不断死磕程序，没有想算法到底是什么样子的，浪费了时间和精力
	vector<int> temp;
	vector<vector<int>> ans;
	
	void dfs(int cur, int n, int k) {
		// 剪枝：temp 长度加上区间 [cur, n] 的长度小于 k，不可能构造出长度为 k 的 temp
		if (temp.size() + (n - cur + 1) < k) {
			return;
		}
		// 记录合法的答案
		if (temp.size() == k) {
			ans.push_back(temp);
			return;
		}
		// 考虑选择当前位置
		temp.push_back(cur);
		dfs(cur + 1, n, k);//假设dfs是平衡的，则递推平衡成立，又因为终值（末端值）也成立，则证明了其平衡性，设计递归，归纳等问题大胆假设，小心求证。
		//广义归纳法：比如某条性质成立，且接下来的性质会维护该条性质的成立，又因为初始情况也成立则该性质对全体都成立。
		temp.pop_back();//此处删除的一定是cur，没有例外，已经证明
		// 考虑不选择当前位置
		dfs(cur + 1, n, k);
	}
	
	vector<vector<int>> combine(int n, int k) {
		dfs(1, n, k);
		return ans;
	}
};
class Solution_2 {//另外一种解法(leet book),采用循环结构，与1完全不一样
public:
	vector<vector<int>> combine(int n, int k) {
		vector<vector<int>> ans;
		vector<int> comb(k, 0);
		int count = 0;
		backtracking(ans, comb, count, 1, n, k);
		return ans;
	}
// 辅函数
	void backtracking(vector<vector<int>>& ans, vector<int>& comb, int& count, int
		pos, int n, int k) {
			if (count == k) {
				ans.push_back(comb);
				return;
			}
			for (int i = pos; i <= n; ++i) {
				comb[count++] = i; // 修改当前节点状态
				backtracking(ans, comb, count, i + 1, n, k); // 递归子节点
				--count; // 回改当前节点状态
			}
		}
};

//二维搜索问题，搜索 ，栅格问题，领域
class Solution_79 {//79. 单词搜索：：给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；否则，返回 false 。
public:
	//state:效率还差很多,还需要很多改进
	//debug:程序正确性问题，与， 程序完全性问题。
	//正确性问题：1.细节问题（粗心大意），可能是编辑错误，如何避免？只能多写。此类问题一旦出现很难排查，只能在一开始就避免
	bool exist(vector<vector<char>>& board, string word) {
	int m = board.size(),n = board[0].size();
		vector<vector<bool>> grid(m,vector<bool>(n,false));
		bool find = false;
		for(int i = 0;i<m;++i){
			for(int j = 0 ;j<n;++j){
				if (!find){
					backtrack(i,j,board,word,0,grid,find);
				}
				else{
					return true;//提升也不大
				}
			}
		}
		return find;
	}
	
	void backtrack(int i,int j,vector<vector<char>>& board,string& word,int pos,vector<vector<bool>>& grid,bool& find){//采用bool backtrack可能更好
		if(i<0 || i>=board.size() || j<0||j>=board[0].size()||find){//bug2: i>board.size()  j>board[0].size()
			return;
		}
		if (grid[i][j] || word[pos] != board[i][j] ){
			return;
		}
		if(pos == word.size()-1){//确保所有的情况都能考虑到，这是递归条件的核心，必须考虑所有可能的情况。
			find = true;//bug1: pos = word.size()-1
			return;
		}
		grid[i][j] = true;
		
		backtrack(i+1,j,board,word,pos+1,grid,find);
		if(!find){backtrack(i-1,j,board,word,pos+1,grid,find);}
		if(!find){backtrack(i,j+1,board,word,pos+1,grid,find);}
		if(!find){backtrack(i,j-1,board,word,pos+1,grid,find);}	//此措施提升不大
		grid[i][j] = false;
	}
};
class Solution_79_1 {//更高效的实现,为什么快那么多？
public:
	bool exist(vector<vector<char>> &board, string word) {
		m = board.size(); // 获取二维矩阵的行数
		n = board[0].size(); // 获取二维矩阵的列数
		for (int i = 0; i < m; ++i) // 遍历二维矩阵的每一行
			for (int j = 0; j < n; ++j) // 遍历二维矩阵的每一列
				if (isFound(board, word.c_str(), i, j)) // 如果找到了目标单词
					return true; // 返回true
		return false; // 否则返回false
	}
private:
	int m; // 存储二维矩阵的行数
	int n; // 存储二维矩阵的列数
	bool isFound(vector<vector<char> > &board, const char *w, int x, int y) {
		if (x < 0 || y < 0 || x >= m || y >= n || board[x][y] == '\0' || *w != board[x][y]) // 如果越界或者当前字符不匹配
			return false; // 返回false
		if (*(w + 1) == '\0') // 如果已经匹配到了单词的末尾
			return true; // 返回true
		char t = board[x][y]; // 保存当前位置的字符
		board[x][y] = '\0'; // 将当前位置的字符置为'\0'，表示已访问过
		if (isFound(board, w + 1, x - 1, y) || isFound(board, w + 1, x + 1, y) || isFound(board, w + 1, x, y - 1) || isFound(board, w + 1, x, y + 1)) // 递归地在上下左右四个方向查找
			return true; // 如果找到了目标单词，返回true
		board[x][y] = t; // 恢复当前位置的字符
		return false; // 否则返回false
	}
};

class Solution_934 {//最短的桥。二维矩阵表示陆地与海洋，有两块陆地，求连接两块陆地的最短的桥。local-v
	//5.7日终于完成，全程独立完成，且是第一次写bfs，值得纪念。
	//全程有很多关于bfs的想法，但很多都有问题，算法逻辑有问题或者算法细节有问题，写了很多无用代码
public:
	int shortestBridge(vector<vector<int>>& grid) {//DFS,BFS都是一种贪心算法，只不过用到了递归函数，还是贪心，有太多可以提升的空间，搜索减枝。
		//总体思路：找到的岛屿标记为2，目标岛屿为1，每次循环扩展一层该层标记为cnt，即桥长度加1，下层标记为cnt+1，由于初始为2，因此最终长度为最大的标记-2。记住
		//算法思路和代码实现还有很大的gap，思路往往很简单，但如何高效实现是个问题，高效源于算法高效以及实现高效。一定要重视代码实现的效率和质量。
		//继续代码重构.
		bool found = false;
		int cnt = 2;
		findFirstIsland(grid);
		while(!found){
			for(int i = 0;i<grid.size();i++){
				for(int j= 0;j<grid[0].size();++j){
					if(grid[i][j]==cnt){
						bfs(grid,i,j,found,cnt);
						if(found)break;
						
					}
				}
				if(found)break;
			}
			cnt++;
		}
		return cnt-3;//???cnt-2
	}
	void findFirstIsland(vector<vector<int>>& grid){//找到第一个岛屿
		int m = grid.size(),n = grid[0].size();
		for(int i = 0;i<m;++i){
			for(int j = 0;j<n;++j){
				if(grid[i][j] != 0){
					dfs(grid,i,j);
					return;
				}
			}
		}
	}
	void dfs(vector<vector<int>>& grid,int i,int j){//dfs搜索第一个岛屿
		if(i<0||i>=grid.size()||j<0||j>=grid[0].size()){
			return;
		}
		if(grid[i][j] == 0 || grid[i][j]==2){
			return;
		}
		if(grid[i][j] == 1){
			grid[i][j] = 2;
			dfs(grid,i+1,j);
			dfs(grid,i-1,j);
			dfs(grid,i,j+1);
			dfs(grid,i,j-1);
		}
		
	}
	void dfs_error_version(vector<vector<int>>& grid,int i,int j){//dfs搜索第一个岛屿
		if(i<0||i>=grid.size()||j<0||j>=grid[0].size()){
			return;
		}
		if(grid[i][j] == 0){
			return;
		}
		grid[i][j] = 2;//对于情形划分不完整，漏掉了一些情况，写代码尤其是dfs,递归一定要将所有情况考虑清楚，考虑完整划分
		dfs(grid,i+1,j);
		dfs(grid,i-1,j);
		dfs(grid,i,j+1);
		dfs(grid,i,j-1);	
	}
	/*void bfs(vector<vector<int>>& grid,int i, int j,bool& found){
	if(i<0||i>=grid.size()||j<0||j>=grid[0].size()){//合法性检查
	return;
	}
	if(grid[i][j] == 2){
	return;
	}
	if(grid[i][j]==1){
	found = true;
	}
	else{
	bfs(grid,i+1,j,found);
	bfs(grid,i-1,j,found);
	bfs(grid,i,j+1,found);
	bfs(grid,i,j-1,found);
	}
	}*/
	void bfs(vector<vector<int>>& grid,int i,int j,bool& found,int&cnt){
		
		vector<int>tmp={-1,0,1,0,-1};
		int i_new,j_new;
		for(int k=0;k<4;++k){
			i_new=i+tmp[k];
			j_new=j+tmp[k+1];
			if(i_new>=0&&i_new<grid.size()&&j_new>=0&&j_new<grid[0].size()){
				if(grid[i_new][j_new]==0){
					grid[i_new][j_new]=cnt+1;
				}
				if(grid[i_new][j_new]==1){
					found = true;
				}
				
			}	
		}
	}
};
class Solution_934_0 {//leetcode版本，性能有待优化
public:
	int shortestBridge(vector<vector<int>>& grid) {//DFS,BFS都是一种贪心算法，只不过用到了递归函数，还是贪心，有太多可以提升的空间。
		bool found = false;
		int cnt = 2;
		findFirstIsland(grid);//找到第一个岛屿
		while(!found){
			for(int i = 0;i<grid.size();i++){
				for(int j= 0;j<grid[0].size();++j){
					if(grid[i][j]==cnt){
						bfs(grid,i,j,found,cnt);
						if(found)break;
						
					}
				}
				if(found)break;
			}
			cnt++;
		}
		return cnt-3;
	}
	void findFirstIsland(vector<vector<int>>& grid){//找到第一个岛屿
		int m = grid.size(),n = grid[0].size();
		for(int i = 0;i<m;++i){
			for(int j = 0;j<n;++j){
				if(grid[i][j] != 0){
					dfs(grid,i,j);
					return;
				}
			}
		}
	}
	void dfs(vector<vector<int>>& grid,int i,int j){//dfs搜索第一个岛屿
		if(i<0||i>=grid.size()||j<0||j>=grid[0].size()){
			return;
		}
		if(grid[i][j] == 0 || grid[i][j]==2){
			return;
		}
		if(grid[i][j] == 1){
			grid[i][j] = 2;
			dfs(grid,i+1,j);
			dfs(grid,i-1,j);
			dfs(grid,i,j+1);
			dfs(grid,i,j-1);
		}
		
	}
	void bfs(vector<vector<int>>& grid,int i,int j,bool& found,int&cnt){//扩展岛屿
		vector<int>tmp={-1,0,1,0,-1};
		int i_new,j_new;
		for(int k=0;k<4;++k){
			i_new=i+tmp[k];
			j_new=j+tmp[k+1];
			if(i_new>=0&&i_new<grid.size()&&j_new>=0&&j_new<grid[0].size()){
				if(grid[i_new][j_new]==0){
					grid[i_new][j_new]=cnt+1;
				}
				if(grid[i_new][j_new]==1){
					found = true;
				}
				
			}	
		}
	}
};
class Solution_130 {//'x','o',将没有被'x'包围的'o'标记为'o'。
public:
	void solve(vector<vector<char>>& board) {
		int m = board.size(),n=board[0].size();
		for(int i=0;i<board.size();++i){
			for(int j=0;j<board[0].size();++j){
				if(board[i][j]=='O'&&(i==0||i==m-1||j==0||j==n-1)){
					dfs(board,i,j);
				}
			}
		}
		//关键有两个：1.从边缘节点如何搜寻，dfs or bfs 2.搜寻的节点恰好是不需要改动的'O'，但是为了标记出来又必须改动，那么后面必须改回去，且避免与要改动的'O'混淆，如何提高效率？
		//一个技巧：在循环遍历并且改变每个格子时，前后改动互不影响，因为仅需要做改动，所以可以遍历一遍高效的改动。在搜寻阶段显然会相互影响，
		for(int i=0;i<board.size();++i){
			for(int j=0;j<board[0].size();++j){
				if(board[i][j]=='O'){
					board[i][j]='X';
				}
				if(board[i][j]=='o'){
					board[i][j]='O';
				}
				
			}
		}
		
	}
	void dfs(vector<vector<char>>& board,int i,int j){
		if(i<0||i>=board.size()||j<0||j>=board[0].size()){
			return;
		}
		if(board[i][j]=='X'){
			return;
		}
		if(board[i][j]=='O'){
			board[i][j]='o';
			dfs(board,i+1,j);
			dfs(board,i-1,j);
			dfs(board,i,j+1);
			dfs(board,i,j-1);
		}//这里少打了个花括号，导致改了15分钟的bug，属于是工程实践类问题，对工程实践还不够熟悉，这也是能力的一部分，要学会检查括号平衡
	}//本程序仅仅这一个bug，一遍完成，没有逻辑问题和任何修改，逻辑清晰且性能优秀，耗时不超过30分钟，这显示了能力的巨大提升，从算法设计到代码实现，特殊情况，细节考虑，效率与质量的统一
	//是目前完成度和完美度最高的程序了
};
int main(){
	//vector<int> nums={1,2,3,4};
	vector <vector <int>> res;
	Solution_2 Sol;
	res = Sol.combine(6,3);
	coutVVector(res);
}
