#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp){
	if(myArray.empty()){
		
		return;
	}else{
		myArray = merge(myArray, myArray.size(), comp);
	}

}

template <class T, class Comparator>
typename std::vector<T> merge(std::vector<T> myvec, int r, Comparator comp){
	
	//std::cout << myvec.size() << std::endl;
	if(r == 1){
		//std::cout << myvec.size() << std::endl;
		return myvec;
	}else{
		int m = r/2;
		//std::cout << m << std::endl;
		std::vector<T> ltemp;
		std::vector<T> rtemp;
		for(int i = 0; i < m; i++){
			ltemp.push_back(myvec[i]);
		}
		for(int j = m; j < r; j++){
			rtemp.push_back(myvec[j]);
		}
		//std::cout << ltemp.size() << " " << rtemp.size() << std::endl;
		std::vector<T> lvec = merge(ltemp, m, comp);
		std::vector<T> rvec = merge(rtemp, r-m, comp);
		std::vector<T> result;
		unsigned i = 0;
		unsigned j = 0;
		
		while(i < lvec.size() && j < rvec.size()){
			if(comp(lvec[i], rvec[j])/*lvec[i] < rvec[j]*/){
				result.push_back(lvec[i]);
				i++;
			}else{
				result.push_back(rvec[j]);
				j++;
			}
		}
		while(i < lvec.size()){
			result.push_back(lvec[i]);
			i++;
		}
		while(j < rvec.size()){
			result.push_back(rvec[j]);
			j++;
		}
		return result;
	}
	

}