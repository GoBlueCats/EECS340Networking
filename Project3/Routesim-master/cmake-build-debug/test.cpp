//
// Created by tian zhang on 6/3/18.
//

int nthUglyNumber(int n) {
    int ugly[n] = {};
    ugly[0] = 1;
    //defien the ptr for fator 2,3,5 pointed to the ugly's position where when it times 2,3 or 5 bigger than the last number
    // of current ugly array
    int ptr2 = 0;
    int ptr3 = 0;
    int ptr5 = 0;
    for(int i = 1; i<n; i++){
        //find the ptr position
        int num2, num3, num5 =0;
        int cptr2, cptr3,cptr5 =0;
        for(cptr2=ptr2; cptr2<i ;cptr2++){
            if( 2*ugly[cptr2] > ugly[i-1])
                num2 = 2*ugly[ptr2];
            break;
        }
        for(cptr3 = ptr3;cptr3<i ;cptr3++){
            if( 3*ugly[cptr3] > ugly[i-1]){
                num3 = 3*ugly[ptr3];
                break;
            }
        }
        for(cptr5 =ptr5; cptr5<i ;cptr5++){
            if(5*ugly[cptr5] > ugly[i-1])
                num5 = 5*ugly[ptr5];
            break;
        }

        //compare three number above and move the ptr;
        if(num2 <= num3 && num2 <= num5){
            ptr2 = cptr2+1;
            ugly[i] = num2;
        }
        else if (num3 <= num2 && num3 <= num5){
            ptr3 = cptr3+1;
            ugly[i] = num3;
        }
        else {
            ptr5 = cptr5+1;
            ugly[i] = num5;
        }

    }
    return ugly[n-1];
}