#include "main.h"
#define MAX 4
class imp_res : public Restaurant
{
public:
	class Node // lưu thông tin gồm tổng energy từ con customer X tới customer đang xét được lưu trong node
	{
	public:
		int sum;
		int curEnergy;
		Node *next;
		Node *prev;
		Node(int a, int d, Node *b, Node *c)
		{
			sum = a;
			curEnergy = d;
			prev = b;
			next = c;
		}
	};
	imp_res(){};
	customer *X = nullptr;
	int curSize = 0;
	int curWaiter = 0;
	customer *headWaiter = nullptr;
	customer *tailWaiter = nullptr;
	customer *head = nullptr;
	customer *tail = nullptr;
	bool checkIfExist(string Name)
	{
		if (X == nullptr)
		{
			return false;
		}
		customer *runner = X;
		while (runner->next != X)
		{
			if (runner->name == Name)
			{
				return true;
			}
			runner = runner->next;
		}
		if (runner->name == Name)
		{
			return true;
		}
		if (headWaiter == nullptr)
		{
			return false;
		}
		runner = headWaiter;
		while (runner != nullptr)
		{
			if (runner->name == Name)
			{
				return true;
			}
			runner = runner->next;
		}
		return false;
	}

	void RED(string name, int energy)
	{
		cout << "RED" << endl;
		if (energy == 0)
		{
			return;
		}
		if (checkIfExist(name) == true)
		{
			return;
		}
		customer *cus = new customer(name, energy, nullptr, nullptr);
		if (curSize < MAX / 2)
		{
			if (X == nullptr)
			{

				X = cus;
				X->next = X;
				X->prev = X;
				curSize++;

				customer *tmp = new customer(name, energy, nullptr, nullptr);
				head = tmp;
				tail = tmp;
				head->next = tail;
				tail->prev = head;
			}
			else if (energy >= X->energy)
			{
				/// add customer to the table
				X->next->prev = cus;
				cus->next = X->next;
				X->next = cus;
				cus->prev = X;
				curSize++;
				X = cus; // update position X
				// add customer to the order list
				customer *tmp = new customer(name, energy, tail, nullptr);
				tail->next = tmp;
				tail = tmp;
			}
			else if (energy < X->energy)
			{
				// add customer to the table
				X->prev->next = cus;
				cus->prev = X->prev;
				cus->next = X;
				X->prev = cus;
				curSize++;
				X = cus; // update positon X
				// add customer to the order list
				customer *tmp = new customer(name, energy, tail, nullptr);
				tail->next = tmp;
				tail = tmp;
			}
		}
		else if (curSize >= MAX / 2 && curSize < MAX)
		{
			int maxRes = -1;
			int realRes = 0;
			customer *runner = X;	 // dùng để duyệt hết bàn tròn
			customer *tmp = nullptr; // dùng để lưu lại con trỏ đích
			// find positon
			while (runner->next != X)
			{
				int res = abs(energy - runner->energy);
				if (res > maxRes)
				{
					maxRes = res;
					realRes = energy - runner->energy;
					tmp = runner;
				}
				runner = runner->next;
			}
			int res = abs(energy - runner->energy);
			if (res > maxRes)
			{
				maxRes = res;
				realRes = energy - runner->energy;
				tmp = runner;
			}
			// add new customer to orderlist
			customer *tmp2 = new customer(name, energy, tail, nullptr);
			tail->next = tmp2;
			tail = tmp2;
			// add new customer to the table
			if (realRes < 0)
			{
				tmp->prev->next = cus;
				cus->prev = tmp->prev;
				cus->next = tmp;
				tmp->prev = cus;
				curSize++;
				X = cus; // update positon X
			}
			else if (realRes > 0)
			{
				tmp->next->prev = cus;
				cus->next = tmp->next;
				tmp->next = cus;
				cus->prev = tmp;
				curSize++;
				X = cus; // update position X
			}
		}
		else if (curSize >= MAX)
		{
			if (curWaiter == MAX)
			{
				free(cus);
				return;
			}
			if (headWaiter == nullptr && tailWaiter == nullptr)
			{
				headWaiter = cus;
				curWaiter++;
			}
			else if (headWaiter != nullptr && tailWaiter == nullptr)
			{
				tailWaiter = cus;
				tailWaiter->prev = head;
				headWaiter->next = tailWaiter;
				curWaiter++;
			}
			else
			{
				tailWaiter->next = cus;
				cus->prev = tailWaiter;
				tailWaiter = cus;
				curWaiter++;
			}
		}
	}
	void BLUE(int num)
	{
		cout << "BLUE" << endl;
		if (num == 0)
		{
			return;
		}
		if (num >= curSize || num >= MAX)
		{
			customer *tmp = X;
			for (int i = 0; i < curSize - 1; i++)
			{
				customer *del = tmp;
				tmp = tmp->next;
				del->prev->next = del->next;
				del->next->prev = del->prev;

				del->next = nullptr;
				del->prev = nullptr;
				delete del;
			}
			delete tmp;
			X = nullptr;
			curSize = 0;
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				customer *tmp = head;
				head = head->next;
				string s = tmp->name;
				tmp->next = nullptr;
				tmp->prev = nullptr;
				delete tmp;
				customer *runner = X;
				for (int j = 0; j < curSize; j++)
				{
					if (runner->name == s)
					{
						if (runner->energy > 0)
						{
							X = runner->next;
						}
						else
						{
							X = runner->prev;
						}

						runner->prev->next = runner->next;
						runner->next->prev = runner->prev;
						runner->next = nullptr;
						runner->prev = nullptr;
						delete runner;
						break;
					}
					else
					{
						runner = runner->next;
					}
				}
			}
			curSize = curSize - num;
		}
		while (curSize < MAX && curWaiter > 0)
		{
			customer *tmp = headWaiter;
			headWaiter = headWaiter->next;
			if (headWaiter != nullptr)
			{
				headWaiter->prev = nullptr;
			}
			tmp->next = nullptr;
			tmp->prev = nullptr;
			RED(tmp->name, tmp->energy);
			curWaiter--;
		}
	}
	/// purpple
	customer *getCustomerInWaitAt(int index)
	{
		customer *tmp = headWaiter;
		for (int i = 0; i < index; i++)
		{
			tmp = tmp->next;
		}
		return tmp;
	}
	void swapWaiter(int left, int right)
	{
		customer *tmpLeft = headWaiter;
		customer *tmpRight = headWaiter;
		for (int i = 0; i < left; i++)
		{
			tmpLeft = tmpLeft->next;
		}
		for (int i = 0; i < right; i++)
		{
			tmpRight = tmpRight->next;
		}
		string Name = tmpLeft->name;
		int Energy = tmpLeft->energy;
		tmpLeft->energy = tmpRight->energy;
		tmpLeft->name = tmpRight->name;
		tmpRight->name = Name;
		tmpRight->energy = Energy;
	}
	void insertionSort4Waiter(int n, int incr, int &count)
	{
		for (int i = incr; i < n; i += incr)
		{
			for (int j = i; j >= incr && abs(getCustomerInWaitAt(j - incr)->energy) < abs(getCustomerInWaitAt(j)->energy); j -= incr)
			{
				swapWaiter(j - 1, j);
				count++;
			}
		}
	}
	void shellsort(int n, int &count)
	{
		for (int i = n / 2; i > 2; i = i / 2)
		{
			for (int j = 0; j < i; j++)
			{
				insertionSort4Waiter(n - j, i, count);
			}
		}
		insertionSort4Waiter(n, 1, count);
	}
	void PURPLE()
	{
		cout << "PURPLE" << endl;
		int index = 0;
		int maxEne = 0;
		customer *runner = headWaiter;
		for (int i = 0; i < curWaiter; i++)
		{
			if (abs(runner->energy) >= maxEne)
			{
				maxEne = abs(runner->energy);
				index = i;
			}
			runner = runner->next;
		}
		int count = 0;
		shellsort(index + 1, count);
		BLUE(count % MAX);
	}

	void REVERSAL()
	{

		cout << "reversal" << endl;
		/// đổi chú thuật sư;
		customer *runner1 = X;
		customer *runner2 = X->next;
		int count1 = 1;
		int count2 = 1;
		while (count1 + count2 <= curSize)
		{

			if (count1 + count2 > curSize)
			{
				break;
			}
			while (runner1->energy < 0)
			{
				runner1 = runner1->prev;
				count1++;
				if (count1 + count2 > curSize)
				{
					break;
				}
			}
			while (runner2->energy < 0)
			{
				runner2 = runner2->next;
				count2++;
				if (count1 + count2 > curSize)
				{
					break;
				}
			}
			if (count1 + count2 > curSize)
			{
				break;
			}
			string a = runner1->name;
			int b = runner1->energy;
			runner1->energy = runner2->energy;
			runner1->name = runner2->name;
			runner2->energy = b;
			runner2->name = a;
			runner1 = runner1->prev;
			count1++;
			runner2 = runner2->next;
			count2++;
		}
		/// đổi oán linh
		runner1 = X;
		runner2 = X->next;
		count1 = 1;
		count2 = 1;
		while (count1 + count2 <= curSize)
		{
			if (count1 + count2 > curSize)
			{
				break;
			}
			while (runner1->energy > 0)
			{
				runner1 = runner1->prev;
				count1++;
				if (count1 + count2 > curSize)
				{
					break;
				}
			}
			while (runner2->energy > 0)
			{
				runner2 = runner2->next;
				count2++;
				if (count1 + count2 > curSize)
				{
					break;
				}
			}
			if (count1 + count2 > curSize)
			{
				break;
			}
			string a = runner1->name;
			int b = runner1->energy;
			runner1->energy = runner2->energy;
			runner1->name = runner2->name;
			runner2->energy = b;
			runner2->name = a;
			runner1 = runner1->prev;
			count1++;
			runner2 = runner2->next;
			count2++;
		}
	}

	void UNLIMITED_VOID()
	{
		cout << "unlimited_void" << endl;

		if (curSize < 4)
		{
			return;
		}
		/// tính tổng các phần tử từ phần tử đầu tới phần tử đang xét
		Node *start = nullptr;
		Node *end = nullptr;
		customer *runner = X;
		for (int i = 0; i < curSize; i++)
		{
			if (start == nullptr && end == nullptr)
			{
				Node *tmp = new Node(runner->energy, runner->energy, nullptr, nullptr);
				runner = runner->next;
				start = tmp;
			}
			else if (start != nullptr && end == nullptr)
			{
				Node *tmp = new Node(runner->energy + start->sum, runner->energy, nullptr, nullptr);
				end = tmp;
				end->prev = start;
				start->next = end;
				runner = runner->next;
			}
			else
			{
				Node *tmp = new Node(runner->energy + end->sum, runner->energy, nullptr, nullptr);
				end->next = tmp;
				tmp->prev = end;
				end = tmp;
				runner = runner->next;
			}
		}
		int minnimum = 999999;
		int resLeft, resRight; // mốc trái pphari của dãy thỏa điều kiện bài
		Node *rightRun = start->next->next->next;
		for (int i = 3; i < curSize; i++)
		{
			Node *leftRun = start;
			for (int j = 0; j <= i - 3; j++)
			{
				int a = rightRun->sum - leftRun->sum + leftRun->curEnergy;
				if (a <= minnimum)
				{
					minnimum = a;
					resLeft = j;
					resRight = i;
				}
				leftRun = leftRun->next;
			}
			rightRun = rightRun->next;
		}
		// kiếm vị trí thằng lớn nhất trong dãy
		customer *left = X;
		customer *right = X;
		int minVal = left->energy;
		customer *remember = X;

		for (int i = 0; i < resLeft; i++)
		{
			if (left->energy < minVal)
			{
				minVal = left->energy;
				remember = left;
			}
			left = left->next;
			right = right->next;
		}
		for (int i = resLeft; i <= resRight; i++)
		{
			if (right->energy < minVal)
			{
				minVal = right->energy;
				remember = right;
			}
			right = right->next;
		}
		right = right->prev;
		runner = remember;
		while (runner != right)
		{
			runner->print();
			runner = runner->next;
		}
		right->print();

		runner = left;
		while (runner != remember)
		{
			runner->print();
			runner = runner->next;
		}
		/// delete NODE;
		Node *del = start;
		for (int i = 0; i < curSize; i++)
		{
			Node *tmp = del;
			del = del->next;
			tmp->prev = nullptr;
			tmp->next = nullptr;
			delete tmp;
		}
		delete del;
	}

	///

	void deleteAndPrintOanLinh()
	{
		/// delete in waiting line
		customer *tmp = tailWaiter;
		while (tmp != nullptr)
		{
			if (tmp->energy < 0)
			{
				customer *del = tmp;
				tmp->print();
				tmp = tmp->prev;
				// delete
				if (del->next == nullptr && del->prev == nullptr)
				{
					delete del;
				}
				else if (del->next == nullptr && del->prev != nullptr)
				{
					del->prev->next = nullptr;
					delete del;
				}
				else if (del->next != nullptr && del->prev == nullptr)
				{
					del->next->prev = nullptr;
					delete del;
				}
				else
				{
					del->prev->next = del->next;
					del->next->prev = del->prev;
					delete del;
				}
				curWaiter--;
			}
			else
			{
				tmp = tmp->prev;
			}
		}

		/// print first +store the name of fist customer in table and delete in table;
		string s = "0"; // name of the first customer
		tmp = tail;
		for (int i = 0; i < curSize; i++)
		{
			if (tmp->energy < 0)
			{
				customer *del = tmp;
				s = tmp->name;
				tmp->print();
				tmp = tmp->prev;
				// delete
				if (del->next == nullptr && del->prev == nullptr)
				{
					delete del;
				}
				else if (del->next == nullptr && del->prev != nullptr)
				{
					del->prev->next = nullptr;
					delete del;
				}
				else if (del->next != nullptr && del->prev == nullptr)
				{
					del->next->prev = nullptr;
					delete del;
				}
				else
				{
					del->prev->next = del->next;
					del->next->prev = del->prev;
					del->next = nullptr;
					del->prev = nullptr;
					delete del;
				}
			}
			else
			{
				tmp = tmp->prev;
			}
		}

		// delete in table;
		if (s == "0")
		{
			return; /// ko tồn tại oán linh trong table;
		}
		customer *remember = nullptr;
		tmp = X;
		int count = 0;
		for (int i = 0; i < curSize; i++)
		{
			if (tmp->energy < 0)
			{
				if (tmp->name != s)
				{
					customer *del = tmp;
					del->prev->next = del->next;
					del->next->prev = del->prev;
					del->next = nullptr;
					del->prev = nullptr;
					delete del;
					count++;
					tmp = tmp->next;
				}
				else if (tmp->name == s)
				{
					remember = tmp;
					tmp = tmp->next;
				}
			}
			else
			{
				tmp = tmp->next;
			}
		}
		curSize -= count;
		X = remember->prev;
		remember->prev->next = remember->next;
		remember->next->prev = remember->prev;
		remember->next = nullptr;
		remember->prev = nullptr;
		delete remember;
		curSize--;
	}
	void deleteAndPrintChuThuatSu()
	{
		/// delete in waiting line
		/// delete in waiting line
		customer *tmp = tailWaiter;
		while (tmp != nullptr)
		{
			if (tmp->energy > 0)
			{
				customer *del = tmp;
				tmp->print();
				tmp = tmp->prev;
				// delete
				if (del->next == nullptr && del->prev == nullptr)
				{
					delete del;
				}
				else if (del->next == nullptr && del->prev != nullptr)
				{
					del->prev->next = nullptr;
					delete del;
				}
				else if (del->next != nullptr && del->prev == nullptr)
				{
					del->next->prev = nullptr;
					delete del;
				}
				else
				{
					del->prev->next = del->next;
					del->next->prev = del->prev;
					delete del;
				}
				curWaiter--;
			}
			else
			{
				tmp = tmp->prev;
			}
		}

		/// print first +store the name of fist customer in table and delete in table;
		string s = "0"; // name of the first customer
		tmp = tail;
		for (int i = 0; i < curSize; i++)
		{
			if (tmp->energy > 0)
			{
				customer *del = tmp;
				s = tmp->name;
				tmp->print();
				tmp = tmp->prev;
				// delete
				if (del->next == nullptr && del->prev == nullptr)
				{
					delete del;
				}
				else if (del->next == nullptr && del->prev != nullptr)
				{
					del->prev->next = nullptr;
					delete del;
				}
				else if (del->next != nullptr && del->prev == nullptr)
				{
					del->next->prev = nullptr;
					delete del;
				}
				else
				{
					del->prev->next = del->next;
					del->next->prev = del->prev;
					del->next = nullptr;
					del->prev = nullptr;
					delete del;
				}
			}
			else
			{
				tmp = tmp->prev;
			}
		}

		// delete in table;
		if (s == "0")
		{
			return; /// ko tồn tại oán linh trong table;
		}
		customer *remember = nullptr;
		tmp = X;
		int count = 0;
		for (int i = 0; i < curSize; i++)
		{
			if (tmp->energy > 0)
			{
				if (tmp->name != s)
				{
					customer *del = tmp;
					del->prev->next = del->next;
					del->next->prev = del->prev;
					del->next = nullptr;
					del->prev = nullptr;
					delete del;
					count++;
					tmp = tmp->next;
				}
				else if (tmp->name == s)
				{
					remember = tmp;
					tmp = tmp->next;
				}
			}
			else
			{
				tmp = tmp->next;
			}
		}
		curSize -= count;
		X = remember->prev;
		remember->prev->next = remember->next;
		remember->next->prev = remember->prev;
		remember->next = nullptr;
		remember->prev = nullptr;
		delete remember;
		curSize--;
	}
	void DOMAIN_EXPANSION()
	{
		cout << "DOMAIN_EXPAINSION" << endl;
		// tính tổng
		int sumDuong = 0;
		int sumAm = 0;
		customer *tmp = X;
		for (int i = 0; i < curSize; i++)
		{
			int a = tmp->energy;
			a > 0 ? sumDuong += a : sumAm += a;
			tmp = tmp->next;
		}
		tmp = headWaiter;
		for (int i = 0; i < curWaiter; i++)
		{
			int a = tmp->energy;
			a > 0 ? sumDuong += a : sumAm += a;
			tmp = tmp->next;
		}

		/// đuổi và in ;
		if (sumDuong >= abs(sumAm))
		{
			// đuổi oán linh;
			// in ra theo  thứ tự gần nhất là cuối danh sách chờ  lên đầu danh sách chờ
			// rồi tới cuối danh sách ăn lên đầu danh sách ăn
			deleteAndPrintOanLinh();
		}
		else
		{
			// đuổi oán linh;
			// in ra theo  thứ tự gần nhất là cuối danh sách chờ  lên đầu danh sách chờ
			// rồi tới cuối danh sách ăn lên đầu danh sách ăn
			deleteAndPrintChuThuatSu();
		}
	}
	void LIGHT(int num)
	{
		cout << "light " << num << endl;
		if (num == 0)
		{
			customer *tmp = headWaiter;
			while (tmp != nullptr)
			{
				tmp->print();
				tmp = tmp->next;
			}
			return;
		}
		if (num > 0)
		{
			customer *tmp = X;
			while (tmp->next != X)
			{
				tmp->print();
				tmp = tmp->next;
			}
			tmp->print();
			return;
		}
		if (num < 0)
		{
			customer *tmp = X;
			while (tmp->prev != X)
			{
				tmp->print();
				tmp = tmp->prev;
			}
			tmp->print();
			return;
		}
	}
};