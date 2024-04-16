#include "../include/BigInt.h"
#include <algorithm>

BigInt::BigInt(){
	sign = '+';
	BigIntString = "0";
}

BigInt::BigInt(int number){
	if (number<0) {
		sign = '-';
		number = abs(number);
	}
	else {
		sign = '+';
	}
	if (number==0) {
		BigIntString.push_back('0');
	}
	while (number!=0) {
		BigIntString.push_back(number%10+'0');
		number = number/10;
	}
}

BigInt::BigInt(std::string number){
    if (number.empty()) {
        throw std::invalid_argument("invalid argument");
    }
    int SignFlag = 0;
    if (number[0]=='+'){
        sign = '+';
        SignFlag = 1;
    }
    if (number[0]=='-'){
        sign = '-';
        SignFlag = 1;
    }
    if (number[0]>='0' && number[0]<='9'){
        sign = '+';
    }
    for (int i = 1; i<=number.size()-SignFlag; i++) {
        if (number[number.size() - i] >= '0' && number[number.size() - i] <= '9') {
            BigIntString.push_back(number[number.size() - i]);
        } else {
            throw std::invalid_argument("invalid argument");
        }
    }
    if (BigIntString.empty()){
        throw std::invalid_argument("invalid argument");
    }
    int k = 0;
    while(k<BigIntString.size() && BigIntString[BigIntString.size()-1-k]=='0'){
        k++;
    }
    if (k == BigIntString.size()){
        BigIntString.push_back('0');
        if (sign == '-'){
            sign = '+';
        }
    }
    BigIntString.resize(BigIntString.size()-k);
}

BigInt::BigInt(const BigInt& BigIntObject){
	BigIntString = BigIntObject.BigIntString;
	sign = BigIntObject.sign;
}

BigInt::~BigInt(){}

BigInt& BigInt::operator=(const BigInt& BigIntObject){
	sign = BigIntObject.sign;
	BigIntString = BigIntObject.BigIntString;
	return *this;

}

BigInt& BigInt::operator++(){
	*this += BigInt(1);
	return *this;
}

BigInt BigInt::operator++(int){
	BigInt save = *this;
	++*this;
	return save;
}

BigInt& BigInt::operator--(){
	*this -= BigInt(1);
	return *this;
}

BigInt BigInt::operator--(int){
	BigInt save = *this;
	--*this;
	return save;
}

BigInt& BigInt::operator+=(const BigInt& BigIntObject){
	size_t size1 = BigIntString.size();
	size_t size2 = BigIntObject.BigIntString.size();
	int remainder = 0;
	if (sign=='+' && BigIntObject.sign=='+' || sign=='-' && BigIntObject.sign=='-') {
		if (size1>=size2) {
			for (int i = 0; i<size2; i++) {
				BigIntString[i] = BigIntString[i]+BigIntObject.BigIntString[i]-'0'+remainder;
				if (BigIntString[i]>'9') {
					BigIntString[i] -= 10;
					remainder = 1;
				}
				else {
					remainder = 0;
				}
			}
			for (int i = size2; i<size1; i++) {
				BigIntString[i] += remainder;
				if (BigIntString[i]>'9') {
					BigIntString[i] -= 10;
					remainder = 1;
				}
				else {
					remainder = 0;
					break;
				}
			}
			if (remainder==1) {
				BigIntString.push_back('1');
			}
		}
		else {
			for (int i = 0; i<size1; i++) {
				BigIntString[i] = BigIntString[i]+BigIntObject.BigIntString[i]-'0'+remainder;
				if (BigIntString[i]>'9') {
					BigIntString[i] -= 10;
					remainder = 1;
				}
				else {
					remainder = 0;
				}
			}
			for (int i = size1; i<size2; i++) {
				BigIntString.push_back(remainder+BigIntObject.BigIntString[i]);
				if (BigIntString[i]>'9') {
					BigIntString[i] -= 10;
					remainder = 1;
				}
				else {
					remainder = 0;
				}
			}
			if (remainder==1) {
				BigIntString.push_back('1');
			}
		}
	}
	else {
		int first_biggest = 1;
		if (size2>size1) {
			sign = BigIntObject.sign;
			first_biggest = 0;
		}
		if (size2==size1) {
			for (int i = size2-1; i>=0; i--) {
				if (BigIntObject.BigIntString[i]>BigIntString[i]) {
					sign = BigIntObject.sign;
					first_biggest = 0;
					break;
				}
				if (BigIntObject.BigIntString[i]<BigIntString[i]) {
					break;
				}
			}
		}
		if (first_biggest==1) {
			for (int i = 0; i<size2; i++) {
				BigIntString[i] = BigIntString[i]-remainder-(BigIntObject.BigIntString[i]-'0');
				remainder = 0;
				if (BigIntString[i]<'0') {
					BigIntString[i] += 10;
					remainder = 1;
				}
			}
			size_t k = size2;
			while (remainder==1) {
				BigIntString[k] -= 1;
				remainder = 0;
				if (BigIntString[k]<'0') {
					BigIntString[k] = '9';
					remainder = 1;
				}
				k++;
			}
			int cnt = 1;
			while (BigIntString[size1-cnt]=='0' && cnt<size1) {
				cnt++;
			}
			BigIntString.resize(size1-cnt+1);
		}
		if (first_biggest==0) {
			for (int i = 0; i<size1; i++) {
				BigIntString[i] = BigIntObject.BigIntString[i]-remainder-(BigIntString[i]-'0');
				remainder = 0;
				if (BigIntString[i]<'0') {
					BigIntString[i] += 10;
					remainder = 1;
				}
			}
			for (int i = size1; i<size2; i++) {
				BigIntString.push_back(BigIntObject.BigIntString[i]-remainder);
				remainder = 0;
				if (BigIntString[i]<'0') {
					BigIntString[i] = '9';
					remainder = 1;
				}
			}
			int cnt = 1;
			while (BigIntString[size2-cnt]=='0') {
				cnt++;
			}
			BigIntString.resize(size2-cnt+1);
		}
	}
	if (BigIntString=="0") {
		sign = '+';
	}
	return *this;
}

BigInt& BigInt::operator*=(const BigInt& BigIntObject){
	size_t size1 = BigIntString.size();
	size_t size2 = BigIntObject.BigIntString.size();
	BigInt NewBigIntString("0");
	NewBigIntString.BigIntString.resize(0);
	NewBigIntString.sign = '+';
	BigInt result("0");

	int remainder = 0;
	if (size1>size2) {
		for (int i = 0; i<size2; i++) {
			if (BigIntObject.BigIntString[i]=='0') {
				NewBigIntString.BigIntString.resize(i+1);
				for (int k = 0; k<=i; k++) {
					NewBigIntString.BigIntString[k] = '0';
				}
				continue;
			}
			for (int j = 0; j<size1; j++) {
				remainder = remainder+(BigIntObject.BigIntString[i]-'0')*(BigIntString[j]-'0');
				NewBigIntString.BigIntString.push_back(remainder%10+'0');
				remainder /= 10;
			}
			if (remainder!=0) {
				NewBigIntString.BigIntString.push_back(remainder%10+'0');
			}
			remainder = 0;
			result += NewBigIntString;
			for (int k = 0; k<=i; k++) {
				NewBigIntString.BigIntString[k] = '0';
			}
			NewBigIntString.BigIntString.resize(i+1);
		}
	}
	else {
		for (int i = 0; i<size1; i++) {
			if (BigIntString[i]=='0') {
				NewBigIntString.BigIntString.resize(i+1);
				for (int k = 0; k<=i; k++) {
					NewBigIntString.BigIntString[k] = '0';
				}
				continue;
			}
			for (int j = 0; j<size2; j++) {
				remainder = remainder+(BigIntObject.BigIntString[j]-'0')*(BigIntString[i]-'0');
				NewBigIntString.BigIntString.push_back(remainder%10+'0');
				remainder /= 10;
			}
			if (remainder!=0) {
				NewBigIntString.BigIntString.push_back(remainder%10+'0');
			}
			remainder = 0;
			result += NewBigIntString;
			for (int k = 0; k<=i; k++) {
				NewBigIntString.BigIntString[k] = '0';
			}
			NewBigIntString.BigIntString.resize(i+1);
		}
	}
	if (sign==BigIntObject.sign) {
		result.sign = '+';
	}
	else {
		result.sign = '-';
	}
	*this = result;
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& BigIntObject){
	return *this += -BigIntObject;
}

BigInt& BigInt::operator/=(const BigInt& BigIntObject){
	if (BigIntObject.BigIntString=="0") {
		throw std::invalid_argument("divisor must not be zero");
	}
	char result_sign = '0';
	std::string result("");
	int size1 = BigIntString.size();
	int place = size1-1;
	int size2 = BigIntObject.BigIntString.size();
	if (sign==BigIntObject.sign) {
		result_sign = '+';
	}
	else {
		result_sign = '-';
	}
	if (BigIntObject.sign=='-') {
		sign = '-';
	}
	else {
		sign = '+';
	}
	int flag;
	char number;
	int SizeDivision = size2;
	int equal = 0;
	while (place+1-size2-(SizeDivision-size2)>=0) {
		if (place+1==size2) {
			equal = 1;
		}
		flag = 0;
		number = '0';
		while (flag==0) {
			for (int i = 0; i<size2; i++) {
				BigIntString[place-size2+1-(SizeDivision-size2)+i] -= (BigIntObject.BigIntString[i]-'0');
				if (BigIntString[place-size2+1-(SizeDivision-size2)+i]<'0') {
					if (place-size2+2-(SizeDivision-size2)+i<=place) {
						BigIntString[place-size2+1-(SizeDivision-size2)+i] += 10;
						BigIntString[place-size2+2-(SizeDivision-size2)+i]--;
					}
				}
			}
			if (BigIntString[place]<'0') {
				for (int i = 0; i<size2; i++) {
					BigIntString[place-size2+1-(SizeDivision-size2)+i] += (BigIntObject.BigIntString[i]-'0');
					if (BigIntString[place-size2+1-(SizeDivision-size2)+i]>'9') {
						BigIntString[place-size2+1-(SizeDivision-size2)+i] -= 10;
						BigIntString[place-size2+2-(SizeDivision-size2)+i]++;
					}
				}
				flag = 1;
			}
			else {
				number++;
			}
		}
		if (number!='0' || equal==1) {
			result.push_back(number);
			while (place>=0 && BigIntString[place]=='0' && (place+1-size2>=0)) {
				place--;
				SizeDivision--;
			}
		}
		if (SizeDivision==size2 && number=='0' && equal!=1 && place!=size1-1) {
			result.push_back('0');
		}
		SizeDivision++;
		while (SizeDivision<size2) {
			SizeDivision++;
			result.push_back('0');
		}
	}
	reverse(result.begin(), result.end());
	sign = result_sign;
	if (result=="" || result=="0") {
		result = '0';
		sign = '+';
	}
	BigIntString = result;
	return *this;
}

BigInt& BigInt::operator^=(const BigInt& BigIntObject){
	std::string string1;
	std::string string2;
	char SaveSign = sign;
	BigInt save = BigIntObject;
	sign = save.sign;
	size_t MinSize= 0;
	size_t MaxSize = 0;
	if (sign=='-') {
		MinSize= *this>save ? 1 : 2;
	}
	else {
		MinSize= *this>save ? 2 : 1;
	}
	if (BigIntString=="0") {
		string1.push_back('0');
	}
	else {
		while (BigIntString>"0") {
			string1.push_back((BigIntString[0]-'0')%2+'0');
			*this /= 2;
		};
	}
	if (save.BigIntString=="0") {
		string2.push_back('0');
	}
	else {
		while (save.BigIntString>"0") {
			string2.push_back((save.BigIntString[0]-'0')%2+'0');
			save /= 2;
		}
	}
	if (MinSize==1) {
		MinSize= string1.size();
		MaxSize = string2.size();
	}
	else {
		MinSize= string2.size();
		MaxSize = string1.size();
	}
	int SquareSize = 1;
	while (SquareSize<=MaxSize || SquareSize<32) {
		SquareSize *= 2;
	}
	string1.resize(SquareSize, '0');
	string2.resize(SquareSize, '0');
	if (SaveSign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='1') {
				string1[k++]--;
			}
			else {
				string1[k]++;
				remainder = 0;
			}
		}
	}
	if (BigIntObject.sign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string2[i] = string2[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string2[k]=='1') {
				string2[k++]--;
			}
			else {
				string2[k]++;
				remainder = 0;
			}
		}
	}
	for (int i = 0; i<SquareSize; i++) {
		string1[i] = string1[i]==string2[i] ? '0' : '1';
	}
	if (string1[SquareSize-1]=='1') {
		SaveSign = '-';
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='0') {
				string1[k++]++;
			}
			else {
				string1[k]--;
				remainder = 0;
			}
		}
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
	}
	else {
		SaveSign = '+';
	}
	int k = 1;
	while (string1[SquareSize-k]=='0' && k<SquareSize) {
		k++;
	}
	*this = 0;
	for (int i = SquareSize-k; i>=0; i--) {
		*this *= 2;
		*this += (string1[i]-'0');
	}
	sign = SaveSign;
	return *this;
}

BigInt& BigInt::operator%=(const BigInt& BigIntObject){
	if (BigIntObject.BigIntString=="0") {
		throw std::invalid_argument("divisor must not be zero");
	}
	int size1 = BigIntString.size();
	int place = size1-1;
	int size2 = BigIntObject.BigIntString.size();
	char sign1 = sign;
	if (BigIntObject.sign=='-') {
		sign = '-';
	}
	else {
		sign = '+';
	}
	int flag = 0;
	char number = '0';
	int SizeDivision = size2;
	int equal = 0;
	while (place+1-size2-(SizeDivision-size2)>=0) {
		if (place+1==size2) {
			equal = 1;
		}
		flag = 0;
		number = '0';
		while (flag==0) {
			for (int i = 0; i<size2; i++) {
				BigIntString[place-size2+1-(SizeDivision-size2)+i] -= (BigIntObject.BigIntString[i]-'0');
				if (BigIntString[place-size2+1-(SizeDivision-size2)+i]<'0') {
					if (place-size2+2-(SizeDivision-size2)+i<=place) {
						BigIntString[place-size2+1-(SizeDivision-size2)+i] += 10;
						BigIntString[place-size2+2-(SizeDivision-size2)+i]--;
					}
				}
			}
			if (BigIntString[place]<'0') {
				for (int i = 0; i<size2; i++) {
					BigIntString[place-size2+1-(SizeDivision-size2)+i] += (BigIntObject.BigIntString[i]-'0');
					if (BigIntString[place-size2+1-(SizeDivision-size2)+i]>'9') {
						BigIntString[place-size2+1-(SizeDivision-size2)+i] -= 10;
						BigIntString[place-size2+2-(SizeDivision-size2)+i]++;
					}
				}
				flag = 1;
			}
			else {
				number++;
			}
		}
		if (number!='0' || equal==1) {
			while (place>=0 && BigIntString[place]=='0' && (place+1-size2>=0)) {
				place--;
				SizeDivision--;
			}
		}
		SizeDivision++;
		while (SizeDivision<size2) {
			SizeDivision++;
		}
	}
	if (size1>=size2) {
		int k = 1;
		while (BigIntString[size2-k]=='0' && k<size2) {
			k++;
		}
		BigIntString.resize(size2-k+1);
	}
	if (sign1=='+') {
		sign = '+';
	}
	else {
		sign = '-';
	}
	return *this;
}

BigInt& BigInt::operator&=(const BigInt& BigIntObject){
	std::string string1;
	std::string string2;
	char SaveSign = sign;
	BigInt save = BigIntObject;
	sign = save.sign;
	size_t MinSize= 0;
	size_t MaxSize = 0;
	if (sign=='-') {
		MinSize= *this>save ? 1 : 2;
	}
	else {
		MinSize= *this>save ? 2 : 1;
	}
	if (BigIntString=="0") {
		string1.push_back('0');
	}
	else {
		while (BigIntString>"0") {
			string1.push_back((BigIntString[0]-'0')%2+'0');
			*this /= 2;
		};
	}
	if (save.BigIntString=="0") {
		string2.push_back('0');
	}
	else {
		while (save.BigIntString>"0") {
			string2.push_back((save.BigIntString[0]-'0')%2+'0');
			save /= 2;
		}
	}
	if (MinSize==1) {
		MinSize= string1.size();
		MaxSize = string2.size();
	}
	else {
		MinSize= string2.size();
		MaxSize = string1.size();
	}
	int SquareSize = 1;
	while (SquareSize<=MaxSize || SquareSize<32) {
		SquareSize *= 2;
	}
	string1.resize(SquareSize, '0');
	string2.resize(SquareSize, '0');
	if (SaveSign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='1') {
				string1[k++]--;
			}
			else {
				string1[k]++;
				remainder = 0;
			}
		}
	}
	if (BigIntObject.sign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string2[i] = string2[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string2[k]=='1') {
				string2[k++]--;
			}
			else {
				string2[k]++;
				remainder = 0;
			}
		}
	}
	for (int i = 0; i<SquareSize; i++) {
		string1[i] = (string1[i]-'0')*(string2[i]-'0')+'0';
	}
	if (string1[SquareSize-1]=='1') {
		SaveSign = '-';
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='0') {
				string1[k++]++;
			}
			else {
				string1[k]--;
				remainder = 0;
			}
		}
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
	}
	else {
		SaveSign = '+';
	}
	int k = 1;
	while (string1[SquareSize-k]=='0' && k<SquareSize) {
		k++;
	}
	*this = 0;
	for (int i = SquareSize-k; i>=0; i--) {
		*this *= 2;
		*this += (string1[i]-'0');
	}
	sign = SaveSign;
	return *this;
}

BigInt& BigInt::operator|=(const BigInt& BigIntObject){
	std::string string1;
	std::string string2;
	char SaveSign = sign;
	BigInt save = BigIntObject;
	sign = save.sign;
	size_t MinSize= 0;
	size_t MaxSize = 0;
	if (sign=='-') {
		MinSize= *this>save ? 1 : 2;
	}
	else {
		MinSize= *this>save ? 2 : 1;
	}
	if (BigIntString=="0") {
		string1.push_back('0');
	}
	else {
		while (BigIntString>"0") {
			string1.push_back((BigIntString[0]-'0')%2+'0');
			*this /= 2;
		};
	}
	if (save.BigIntString=="0") {
		string2.push_back('0');
	}
	else {
		while (save.BigIntString>"0") {
			string2.push_back((save.BigIntString[0]-'0')%2+'0');
			save /= 2;
		}
	}
	if (MinSize==1) {
		MinSize= string1.size();
		MaxSize = string2.size();
	}
	else {
		MinSize= string2.size();
		MaxSize = string1.size();
	}
	int SquareSize = 1;
	while (SquareSize<=MaxSize || SquareSize<32) {
		SquareSize *= 2;
	}
	string1.resize(SquareSize, '0');
	string2.resize(SquareSize, '0');
	if (SaveSign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='1') {
				string1[k++]--;
			}
			else {
				string1[k]++;
				remainder = 0;
			}
		}
	}
	if (BigIntObject.sign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string2[i] = string2[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string2[k]=='1') {
				string2[k++]--;
			}
			else {
				string2[k]++;
				remainder = 0;
			}
		}
	}
	for (int i = 0; i<SquareSize; i++) {
		string1[i] = string1[i]==string2[i] ? string1[i] : '1';
	}
	if (string1[SquareSize-1]=='1') {
		SaveSign = '-';
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='0') {
				string1[k++]++;
			}
			else {
				string1[k]--;
				remainder = 0;
			}
		}
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
	}
	else {
		SaveSign = '+';
	}
	int k = 1;
	while (string1[SquareSize-k]=='0' && k<SquareSize) {
		k++;
	}
	*this = 0;
	for (int i = SquareSize-k; i>=0; i--) {
		*this *= 2;
		*this += (string1[i]-'0');
	}
	sign = SaveSign;
	return *this;
}

BigInt BigInt::operator~() const{
	BigInt save = *this;
	char SaveSign ='0';
	std::string string1 ="";
	size_t size = 0;
	if (BigIntString=="0") {
		string1.push_back('0');
	}
	else {
		while (save.BigIntString>"0") {
			string1.push_back((save.BigIntString[0]-'0')%2+'0');
			save /= 2;
		};
	}
	size = string1.size();
	int SquareSize = 1;
	while (SquareSize<=size || SquareSize<32) {
		SquareSize *= 2;
	}
	string1.resize(SquareSize, '0');
	if (sign=='-') {
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='1') {
				string1[k++]--;
			}
			else {
				string1[k]++;
				remainder = 0;
			}
		}
	}
	for (int i = 0; i<SquareSize; i++) {
		string1[i] = string1[i]=='0' ? '1' : '0';
	}
	if (string1[SquareSize-1]=='1') {
		SaveSign = '-';
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='0') {
				string1[k++]++;
			}
			else {
				string1[k]--;
				remainder = 0;
			}
		}
		for (int i = 0; i<SquareSize; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
	}
	else {
		SaveSign = '+';
	}
	int k = 1;
	while (string1[SquareSize-k]=='0' && k<SquareSize) {
		k++;
	}
	save = 0;
	for (int i = SquareSize-k; i>=0; i--) {
		save *= 2;
		save += (string1[i]-'0');
	}
	save.sign = SaveSign;
	return save;
}

BigInt BigInt::operator+() const{
	return *this;
}

BigInt BigInt::operator-() const{
	BigInt save = *this;
	if (save.BigIntString!="0") {
		if (save.sign=='+') {
			save.sign = '-';
		}
		else {
			save.sign = '+';
		}
	}
	return save;
}

bool BigInt::operator==(const BigInt& BigIntObject) const{
	return (sign==BigIntObject.sign && BigIntString==BigIntObject.BigIntString);
}

bool BigInt::operator!=(const BigInt& BigIntObject) const{
    return !(*this == BigIntObject);
}

bool BigInt::operator<(const BigInt& BigIntObject) const{
	size_t size1 = size();
	size_t size2 = BigIntObject.size();
	if (sign=='-' && BigIntObject.sign=='+') {
		return 1;
	}
	if (sign=='+' && BigIntObject.sign=='-') {
		return 0;
	}
	if (size1<size2) {
		if (sign=='+') {
			return 1;
		}
		else {
			return 0;
		}
	}
	if (size1>size2) {
		if (sign=='-') {
			return 1;
		}
		else {
			return 0;
		}
	}
	for (int i = size1-1; i>=0; i--) {
		if (BigIntString[i]<BigIntObject.BigIntString[i]) {
			if (sign=='+') {
				return 1;
			}
			else {
				return 0;
			}
		}
		if (BigIntString[i]>BigIntObject.BigIntString[i]) {
			if (sign=='+') {
				return 0;
			}
			else {
				return 1;
			}
		}
	}
	return 0;
}

bool BigInt::operator>(const BigInt& BigIntObject) const{
    return !(*this <= BigIntObject);
}

bool BigInt::operator<=(const BigInt& BigIntObject) const{
    return *this < BigIntObject|| *this == BigIntObject;
}

bool BigInt::operator>=(const BigInt& BigIntObject) const{
    return !(*this < BigIntObject);
}

BigInt::operator int() const{
	BigInt save = *this;
	char SaveSign = sign;
	std::string string1;
	if (BigIntString=="0") {
		string1.push_back('0');
	}
	else {
		while (save.BigIntString>"0") {
			string1.push_back((save.BigIntString[0]-'0')%2+'0');
			save /= 2;
		};
	}
	string1.resize(32, '0');
	if (sign=='-') {
		for (int i = 0; i<32; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
		int remainder = 1;
		int k = 0;
		while (remainder!=0 && k<=31) {
			if (string1[k]=='1') {
				string1[k++]--;
			}
			else {
				string1[k]++;
				remainder = 0;
			}
		}
	}
	if (string1[31]=='1') {
		SaveSign = '-';
		int remainder = 1;
		int k = 0;
		while (remainder!=0) {
			if (string1[k]=='0') {
				string1[k++]++;
			}
			else {
				string1[k]--;
				remainder = 0;
			}
		}
		for (int i = 0; i<32; i++) {
			string1[i] = string1[i]=='0' ? '1' : '0';
		}
	}
	else {
		SaveSign = '+';
	}
	int sum = 0;
	for (int i = 31; i>=0; i--) {
		sum *= 2;
		sum += (string1[i]-'0');
	}
	if (SaveSign=='-') {
		sum = -sum;
	}
	return sum;
}

BigInt::operator std::string() const{
	std::string save;
	save = BigIntString;
	if (*this<BigInt(0)){
		save.push_back('-');
	}
	reverse(save.begin(),save.end());
	return save;
}

size_t BigInt::size() const{
	return BigIntString.size()+1;
}

BigInt operator+(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt += BigIntSecond;
	return FirstBigInt;
}

BigInt operator-(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt -= BigIntSecond;
	return FirstBigInt;
}

BigInt operator*(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt *= BigIntSecond;
	return FirstBigInt;
}

BigInt operator/(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt /= BigIntSecond;
	return FirstBigInt;
}

BigInt operator^(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt ^= BigIntSecond;
	return FirstBigInt;
}

BigInt operator%(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt %= BigIntSecond;
	return FirstBigInt;
}

BigInt operator&(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt &= BigIntSecond;
	return FirstBigInt;
}

BigInt operator|(const BigInt& BigIntFirst, const BigInt& BigIntSecond){
	BigInt FirstBigInt = BigIntFirst;
	FirstBigInt |= BigIntSecond;
	return FirstBigInt;
}

std::ostream& operator<<(std::ostream& o, BigInt& i){
	return o << std::string(i);
}

