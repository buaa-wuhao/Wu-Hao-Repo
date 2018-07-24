#include <vector>
#include <iostream>
#include <algorithm>
using std::vector;

void dfs(int ts,const vector<int>& score,
	     int& nums,int start)
{
	if(ts < 0) return;
	if(ts == 0)
	{
		nums++;
		//for(int i=0;i<path.size();i++)
		{
			//std::cout<<path[i]<<" ";
		}
		//std::cout<<std::endl;
		return;
	} 

	for(int i=start;i < score.size();i++)
	{
		//path.push_back(score[i]);
		dfs(ts-score[i],score,nums,i);
		//path.pop_back();
	}
	return;
}

int NumCombinationsForFinalScore(int final_score,
                                 const vector<int>& individual_play_scores) {
  // Implement this placeholder.
  int nums = 0;
  //std::sort(individual_play_scores.begin(),individual_play_scores.end());
  dfs(final_score,individual_play_scores,nums,0);
  return nums;
}


#include "test_framework/test_utils_generic_main.h"

int main(int argc, char* argv[]) {
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  generic_test_main(argc, argv, param_names, "number_of_score_combinations.tsv",
                    &NumCombinationsForFinalScore);
  return 0;
}
