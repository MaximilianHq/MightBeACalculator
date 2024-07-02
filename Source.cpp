#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <functional>
#include <algorithm>
#include <format>

using namespace std;

namespace graph_color {
	const string axis_x = "\033[31m";	// x-axis
	const string axis_y = "\033[31m";	// y-axis
	const string function = "\033[32m";	// graph
	const string intersect = "\033[35m";// intersect (graph)
	const string root = "\033[34m";		// x = 0
	const string end = "\033[0m";		// end of color change
}

class Graph {
public:

	Graph(int range) : range(range) {
		// initialize the graph filling every vector with empty space
		graph = vector<vector<string>>(rows, vector<string>(cols, " "));
	}

	void initializeGraph() {
		int range_x = 0;
		int range_y = 0;

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				// set numbers on x-axis
				if (j == half_range) {
					graph[i][j] = graph_color::axis_y + to_string(range_y) + graph_color::end;

					// While on the positive side of 0
					if (i < half_range)
						range_y--;
					// While on the negative side of 0
					else
						range_y++;

					// switch at tens
					if (range_y < 0) // 0 9 8 7 ...
						range_y = 9;
					else if (range_y > 9) // 0 1 2 3 ...
						range_y = 0;

					// move range_x once at center (replaced by y-axis)
					if (i == half_range)
						range_x++;
				}
				// set numbers on y-axis
				else if (i == half_range) {
					graph[i][j] = graph_color::axis_x + to_string(range_x) + graph_color::end;

					// While on the negative side of 0
					if (j < half_range)
						range_x--;
					// While on the positive side of 0
					else
						range_x++;

					// switch at tens
					if (range_x < 0) // 0 9 8 7 ...
						range_x = 9;
					else if (range_x > 9) // 0 1 2 3 ...
						range_x = 0;
				}
			}
	}

	void plotGraph() {
		// iterate through and display all
		for (int i = 0; i <= range; i++) {
			for (int j = 0; j <= range; j++) {
				// separate with " "
				cout << graph[i][j] << " ";
			}
			// row-break
			cout << endl;
		}
	}

	void calculateFunctionPoints(vector<double> coeff) {
		// store function variables
		function_variables.push_back(coeff);
		// reverse vector to be able to loop with i
		reverse(coeff.begin(), coeff.end());

		vector<map<string, double>> func;

		for (int x = -range / 2; x <= range / 2; x++) {
			map<string, double> point;
			point["x"] = x;
			for (int i = coeff.size() - 1; i >= 0; i--) {
				// v{a, b, c} => v_r = {c, b, a}. v_r[i] = a; x^i; i index of a = 2
				point["y"] += round(coeff[i] * pow(x, i));
			}
			// add point to function
			func.push_back(point);
		}
		// add function to function list
		functions.push_back(func);
	}

	void plotFunctions() {
		for (auto& func : functions) // iterate through every function
			for (int i = 0; i < rows; i++)
				for (int j = 0; j < cols; j++)
					for (auto& point : func)
						// y becomes larger as i goes towards 0, while x becomes larger as i goes towards infinity
						if (i == -point["y"] + half_range and j == point["x"] + half_range) {
							if (i == half_range)
								graph[i][j] = graph_color::root + "0" + graph_color::end;
							else if (graph[i][j] != " " and j != half_range) // <-- prevent y axis override
								graph[i][j] = graph_color::intersect + "0" + graph_color::end;
							else
								graph[i][j] = graph_color::function + "0" + graph_color::end;
						}
	}

	void displayFunctions() {
		int func_index = 0;
		for (auto& func : functions) {
			cout << "\nFunction " << func_index + 1 << endl;
			cout << "f(x):";
			for (auto& var : function_variables[func_index])
				cout << " " << var;
			cout << endl;
		}
	}

	bool deleteFunction(int& i) {
		if (i >= 0 && i < functions.size()) {
			// erase vector at specified index
			functions.erase(functions.begin() + i);
			return true;
		}
		else {
			std::cout << "Index is out of bounds." << std::endl;
			return false;
		}
	}

private:
	int range;
	int rows = range + 1;
	int cols = range + 1;
	int half_range = range / 2;

	vector<vector<map<string, double>>> functions;
	vector<vector<double>> function_variables;

	vector<vector<string>> graph;
};

void displayMenu() {
	cout << "\033[1;36m"
		"\n=============="
		"\n1. New function"
		"\n2. Edit / Delete"
		"\n3. Plot Graph"
		"\n4. Exit"
		"\n=============="
		"\033[0m" << endl;
}

void displayMenuEdit() {
	cout << "\033[1;31m"
		"\n=============="
		"\n1. Edit"
		"\n2. Delete"
		"\n3. Reset"
		"\n4. Exit"
		"\n=============="
		"\033[0m" << endl;
}

template <typename T>
void InputBracket(T& cinput) {
	cout << ">>> ";
	cin >> cinput;
}

void GraphEditLogic(Graph& calculator) {
	int input;
	const int EXIT = 4;

	do {
		calculator.displayFunctions();
		displayMenuEdit();

		InputBracket(input);

		switch (input)
		{
		case 1: {
		}
		case 2: {
			cout << "Select function to delete:" << endl;
			do {
				InputBracket(input);
			} while (!calculator.deleteFunction(input));
		}
		case 3: {
		}
		default:
			break;
		}
	} while (input != EXIT);
}

int main() {
	const int RANGE = 40;

	Graph calculator(RANGE);

	const int MENU_EXIT = 4;
	int menu_choice = MENU_EXIT;

	do {
		displayMenu();

		InputBracket(menu_choice);

		switch (menu_choice)
		{
		case 1: {
			double input;
			vector<double> function_var;

			cout << "Enter variables, 's' to stop, ... ax ^ 2 + bx + c" << endl;

			do {
				InputBracket(input);

				if (cin.fail()) {
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					break;
				}
				else
					function_var.push_back(input);
			} while (true);

			calculator.calculateFunctionPoints(function_var);

			break;
		}
		case 2: {
			GraphEditLogic(calculator);

			break;
		}
		case 3: {
			calculator.initializeGraph();
			calculator.plotFunctions();
			calculator.plotGraph();

			break;
		}
		default:
			break;
		}
	} while (menu_choice != MENU_EXIT);

	return 0;
}