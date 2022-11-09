#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAP_HEIGHT = 10;
const int MAP_WIDTH = 15;

string intToStr(int num)
{
    string result = "";
    if (num == 0)
    {
        result += 48;
        return result;
    }

    while (num > 0)
    {
        result += (num % 10) + 48;
        num /= 10;
    }
    string tluser = "";
    for (int i = result.size() - 1; i >= 0; i--)
    {
        tluser += result[i];
    }
    return tluser;
}

void drawScreen(const int arri_map[MAP_HEIGHT][MAP_WIDTH], int i_mapSize, int posX, int posY)
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int S = 0; S < i_mapSize; S++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                if (i == posY && j == posX)
                {
                    for (int s = 0; s < i_mapSize; s++)
                    {
                        cout << '*';
                    }
                }
                else
                {
                    switch (arri_map[i][j])
                    {
                    case 0:
                        for (int s = 0; s < i_mapSize; s++)
                        {
                            cout << ' ';
                        }
                        break;
                    case 1:
                        for (int s = 0; s < i_mapSize; s++)
                        {
                            cout << '#';
                        }
                        break;
                    case 5:
                        for (int s = 0; s < i_mapSize; s++)
                        {
                            cout << '-';
                        }
                        break;
                    case 8:
                        for (int s = 0; s < i_mapSize; s++)
                        {
                            cout << '=';
                        }
                        break;
                    }
                }
            }
            cout << endl;
        }
    }
}

class C_MazeMap
{
public:
    const int arri_map[MAP_HEIGHT][MAP_WIDTH] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
                                                 8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
                                                 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
                                                 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
                                                 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5,
                                                 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
                                                 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    ;

    const int i_StartX = 14;
    const int i_StartY = 7;

    const int i_ExitX = 0;
    const int i_ExitY = 2;

    // public:
    double TestRoute(const vector<int> &vecPath)
    {
        int posX = i_StartX;
        int posY = i_StartY;

        for (int i = 0; i < vecPath.size(); i++)
        {
            switch (vecPath.at(i))
            {
            case 0:
                if (arri_map[posY - 1][posX] != 1 && posY - 1 <= MAP_HEIGHT && posY - 1 >= 0 && posX <= MAP_WIDTH && posX >= 0)
                {
                    posY--;
                }
                break;
            case 1:
                if (arri_map[posY][posX + 1] != 1 && posX + 1 <= MAP_WIDTH && posX + 1 >= 0 && posY <= MAP_HEIGHT && posY >= 0)
                {
                    posX++;
                }
                break;
            case 2:
                if (arri_map[posY + 1][posX] != 1 && posY + 1 <= MAP_HEIGHT && posY + 1 >= 0 && posX <= MAP_WIDTH && posX >= 0)
                {
                    posY++;
                }
                break;
            case 3:
                if (arri_map[posY][posX - 1] != 1 && posX - 1 <= MAP_WIDTH && posX - 1 >= 0 && posY <= MAP_HEIGHT && posY >= 0)
                {
                    posX--;
                }
                break;
            }
        }

        posX = abs(posX - i_ExitX);
        posY = abs(posY - i_ExitY);
        return 1 / (double(posX + posY + 1));
    }
};

struct S_Genome
{
    vector<int> vec_Bits;
    double d_Fitness;
    S_Genome() : d_Fitness(0) {}
    S_Genome(const int num_bits) : d_Fitness(0)
    {
        for (int i = 0; i < num_bits; i++)
        {
            vec_Bits.push_back(rand() % 2);
        }
    }
};

class C_GenAlgo
{
public:
    // the population of genomes
    vector<S_Genome> vec_Genomes;

    // the size of population
    int i_PopSize;

    double d_CrossoverRate;
    double d_MutationRate;

    // the num of bits per chromosome
    int i_ChromoLength;

    // the num of bits per gene
    int i_GeneLength;

    int i_FittestGenome;

    double d_BestFitnessScore;

    double d_TotalFitnessScore;

    int i_Generation;

    // initialize instance of Map
    C_MazeMap M_MazeReal;

    // initialize instance of Map for display
    C_MazeMap M_MazeMind;

    // is current run in progress
    bool b_Running;

    void Mutate(vector<int> &vecBits)
    {
        for (int curBit = 0; curBit < vecBits.size(); curBit++)
        {
            // flip this bit?
            if (((double)rand() / RAND_MAX) < d_MutationRate)
            {
                // flip the bit
                vecBits[curBit] = !vecBits[curBit];
            }
        } // next bit
    }

    void Crossover(const vector<int> &mom, const vector<int> &dad, vector<int> &baby1, vector<int> &baby2)
    {
        if (((double)rand() / RAND_MAX) > d_CrossoverRate || mom == dad)
        {
            baby1 = mom;
            baby2 = dad;

            return;
        }
        else
        {
            int cp = (rand() % i_ChromoLength);

            for (int i = 0; i < cp; i++)
            {
                baby1.push_back(mom[i]);
                baby2.push_back(dad[i]);
            }

            for (int i = cp; i < mom.size(); i++)
            {
                baby1.push_back(dad[i]);
                baby2.push_back(mom[i]);
            }
        }
    }

    S_Genome &RouletteWheelSelection()
    {
        double Slice = ((double)rand() / RAND_MAX) * d_TotalFitnessScore;
        double Total = 0;
        int SelectedGenome = 0;
        for (int i = 0; i < i_PopSize; i++)
        {
            Total += vec_Genomes[i].d_Fitness;
            if (Total > Slice)
            {
                SelectedGenome = i;
                break;
            }
        }
        return vec_Genomes[SelectedGenome];
    }

    // sets Fittest Genome, Best FS, Total FS
    void UpdateFitnessScores()
    {
        double maximum = 0;
        int location = 0;
        double total = 0;
        for (int i = 0; i < vec_Genomes.size(); i++)
        {
            vec_Genomes.at(i).d_Fitness = M_MazeReal.TestRoute(Decode(vec_Genomes.at(i).vec_Bits));
            if (vec_Genomes.at(i).d_Fitness > maximum)
            {
                maximum = vec_Genomes.at(i).d_Fitness;
                location = i;
            }
            total += vec_Genomes.at(i).d_Fitness;
        }

        d_BestFitnessScore = maximum;
        i_FittestGenome = location;
        d_TotalFitnessScore = total;
        SaveGenerationData();
    }

    // converts vector of bits to directions
    vector<int> Decode(const vector<int> &bits)
    {
        vector<int> result;
        for (int i = 0; i < bits.size(); i += 2)
        {
            vector<int> direction = {bits.at(i), bits.at(i + 1)};
            result.push_back(BinToInt(direction));
        }
        return result;
    }

    // convert vector of bits to decimal. Used by Decode.
    int BinToInt(const vector<int> v)
    {
        int total = 0;
        for (int i = v.size() - 1; i >= 0; i--)
        {
            total += v[i] * pow(2, (v.size() - 1 - i));
        }
        return total;
    }

    // creates a start population of random bit strings
    void CreateStartPopulation()
    {
        for (int i = 0; i < i_PopSize; i++)
        {
            S_Genome newGenome(i_ChromoLength);
            vec_Genomes.push_back(newGenome);
        }
    }

    // public:
    C_GenAlgo(
        double cross_rat,
        double mut_rat,
        int pop_size,
        int num_bits,
        int gene_len) : d_CrossoverRate(cross_rat),
                        d_MutationRate(mut_rat),
                        i_PopSize(pop_size),
                        i_ChromoLength(num_bits),
                        d_TotalFitnessScore(0.0),
                        i_Generation(0),
                        i_GeneLength(gene_len),
                        b_Running(false)
    {
        CreateStartPopulation();
    }

    void Run()
    {
        cout << "Population initialized..." << endl;
        char a = 'Y';
        do
        {
            Epoch();
            cout << "Best Fitness Score: " << d_BestFitnessScore << endl;
            cout << "Best Genome: ";
            for (int i = 0; i < vec_Genomes.at(i_FittestGenome).vec_Bits.size(); i++)
            {
                cout << vec_Genomes.at(i_FittestGenome).vec_Bits.at(i);
            }
            cout << endl;
            if (i_Generation % 5 == 0)
            {
                cout << "Run next 5 Generations (Y/N)? >";
                cin >> a;
                cout << endl;
                cout << endl;
            }
        } while (a == 'Y');

        cout << "Program Complete. Exit Success" << endl;
    };

    void Epoch()
    {
        UpdateFitnessScores();

        // Now to create a new population
        int NewBabies = 0;
        // create some storage for the baby genomes
        vector<S_Genome> BabyGenomes;
        while (NewBabies < i_PopSize)
        {
            // select 2 parents
            S_Genome mom = RouletteWheelSelection();
            S_Genome dad = RouletteWheelSelection();

            // operator - crossover
            S_Genome baby1, baby2;
            Crossover(mom.vec_Bits, dad.vec_Bits, baby1.vec_Bits, baby2.vec_Bits);
            // operator - mutate
            Mutate(baby1.vec_Bits);
            Mutate(baby2.vec_Bits);
            // add to new population
            BabyGenomes.push_back(baby1);
            BabyGenomes.push_back(baby2);
            NewBabies += 2;
        }
        vec_Genomes = BabyGenomes;
        i_Generation++;
    }

    void Render();

    void SaveGenerationData()
    {
        string fileName = "data\\Generation_" + intToStr(i_Generation) + "_file.txt";
        cout << "Saving Generation Info to " << fileName << "..." << endl;
        fstream saveFile;
        saveFile.open(fileName, ios::out);
        if (saveFile)
        {
            cout << "File Opened..." << endl;
            saveFile << i_Generation << ' ' << i_FittestGenome << ' ' << d_BestFitnessScore << ' ' << endl;
            for (S_Genome genome : vec_Genomes)
            {
                saveFile << ' ' << genome.d_Fitness << ' ';
                for (int j = 0; j < Decode(genome.vec_Bits).size(); j++)
                {
                    saveFile << Decode(genome.vec_Bits).at(j);
                }
                saveFile << endl;
            }
            cout << "Saved Generation: " << i_Generation << endl;
            saveFile.close();
        }
    }

    // accessor methods
    int Generation() { return i_Generation; }
    int GetFittest() { return i_FittestGenome; }
    bool Started() { return b_Running; }
    void Stop() { b_Running = false; }
};

int main()
{
    cout << "Program started..." << endl;
    srand(time(0));
    C_GenAlgo bob(0.7, 0.0001, 140, 70, 2);

    bob.Run();
    return 0;
}