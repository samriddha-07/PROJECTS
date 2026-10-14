#include<iostream>
#include<vector>
#include<string>
#include<limits>
using namespace std;

class gate          //INPUT ABSTRACT BASE CLASS
{
    protected:
    bool input1{false};
    bool input2{false};
    bool iunary{false};

    public:
    explicit gate(bool unary=false) : iunary(unary) {}
    
    virtual ~gate() {}
    void setinput1(bool val)
    {
        input1=val;
    }
    void setinput2(bool val)
    {
        input2=val;
    }
    bool setunary() const
    {
        return iunary;
    }
    virtual string getname() const =0;
    virtual bool evaluate() const =0;
};
                                //INHERITED DERIVED CLASSES
class ANDGate: public gate
{
    public:
    string getname() const override
    {
        return "AND";
    }
    bool evaluate() const override
    {
        return {input1 && input2};
    }
};
class ORGate: public gate
{
    public:
    string getname() const override
    {
        return "OR";
    }
    bool evaluate() const override
    {
        return {input1 || input2};
    }
};
class NOTGate: public gate
{
    public:
    NOTGate() : gate(true) {}
    string getname() const override
    {
        return "NOT";
    }
    bool evaluate() const override
    {
        return {!input1};
    }
};
class XORGate: public gate
{
    public:
    string getname() const override
    {
        return "XOR";
    }
    bool evaluate() const override
    {
        return {input1 != input2};
    }
};
class XNORGate: public gate
{
    public:
    string getname() const override
    {
        return "XNOR";
    }
    bool evaluate() const override
    {
        return {input1 == input2};
    }
};
class NANDGate: public gate
{
    public:
    string getname() const override
    {
        return "NAND";
    }
    bool evaluate() const override
    {
        return {!(input1 && input2)};
    }
};
class NORGate: public gate
{
    public:
    string getname() const override
    {
        return "NOR";
    }
    bool evaluate() const override
    {
        return {!(input1 || input2)};
    }
};
                        //CIRCUIT EVALUATION
class circuit
{
    private:
    int totallayer{3};
    vector<gate*> layer1;
    vector<gate*> layer2;
    gate* outputgate{nullptr};
    public:
    explicit circuit(int layers) : totallayer(layers) {}
    ~circuit()
    {
        for(gate*g: layer1)
        {
            delete g;
        }
        for(gate*g:layer2)
        {
            delete g;
        }
        if(outputgate)
        {
            delete outputgate;
        }
    }
    void addlayer1(gate*g)
    {
        if(g) layer1.push_back(g);
    }
    void addlayer2(gate*g)
    {
        if(g) layer2.push_back(g);
    }
    void setoutputgate(gate*g)
    {
        if(g) outputgate=g;
    }
    bool iscomplete() const
    {
        if(totallayer==1) return outputgate!=nullptr;
        if(totallayer==2) return layer1.size()==2 && outputgate!= nullptr;
        if(totallayer==3) return layer1.size()==4 && layer2.size()==2 && outputgate !=nullptr;
        return false;
    }
    void runcircuit(int(*getBinaryFunc)(const string&))
    {
        if(!iscomplete())
        {
            cout<<"Error: Circuit topology is incomplete"<<endl;
            return;
        }
        bool finaloutput=false;
        //1 layer ckt:
        if(totallayer==1)
        {
            cout<<"\n---Evaluating layer 1(output gate)---"<<endl;
            outputgate->setinput1(getBinaryFunc(" Input A (0/1): "));
            outputgate->setinput2(getBinaryFunc(" Input B (0/1): "));
            finaloutput = outputgate->evaluate();
        }
        //2 layer ckt:
        else if(totallayer==2)
        {
            cout<<"\n---Enter Inputs for Layer 1---"<<endl;
            vector<bool> layer1outputs;
            for(size_t i =0;i<layer1.size();i++)
            {
                cout << "\n Gate " << (i + 1) << " [" << layer1[i]->getname() << "]:\n";
                layer1[i]->setinput1(getBinaryFunc(" Input A (0/1): "));
                layer1[i]->setinput2(getBinaryFunc(" Input B (0/1): "));
                bool out=layer1[i]->evaluate();
                layer1outputs.push_back(out);
                cout<<" ->Output: "<<out<<endl;
            }
            cout<<"\n---Evaluating layer 2 (output gate)--"<<endl;
            outputgate->setinput1(layer1outputs[0]);
            outputgate->setinput2(layer1outputs[1]);
            cout<<"Gate 3["<<outputgate->getname()<<"] receives Input A: "<<layer1outputs[0]<<endl;
            cout<<"Gate 3["<<outputgate->getname()<<"] receives Input B: "<<layer1outputs[1]<<endl;
            finaloutput = outputgate->evaluate();
        }
        //3 layer ckt:
        else if(totallayer==3)
        {
            cout<<"\n---Enter Inputs for layer 1---"<<endl;
            vector<bool> layer1outputs;
            for(size_t i=0;i<layer1.size();i++)
            {
                cout<<"\n Gate "<<(i+1)<<"[" << layer1[i]->getname()<<"]:\n";
                layer1[i]->setinput1(getBinaryFunc(" Input A (0/1): "));
                layer1[i]->setinput2(getBinaryFunc(" Input B (0/1): "));
                bool out = layer1[i]->evaluate();
                layer1outputs.push_back(out);
                cout<<" -> Output: "<<out<<endl;
            }
            cout<<"\n---Evaluating layer 2---"<<endl;
            vector<bool> layer2outputs;
            for(size_t i=0;i<layer2.size();i++)
            {
                size_t inputindex1=i*2;
                size_t inputindex2=i*2+1;
                layer2[i]->setinput1(layer1outputs[inputindex1]);
                layer2[i]->setinput2(layer1outputs[inputindex2]);
                cout<<"Gate "<<i+5<<"["<<layer2[i]->getname() <<"] receives Input A: "<<layer1outputs[inputindex1]<<endl;
                cout<<"Gate "<<i+5<<"["<<layer2[i]->getname() <<"] receives Input B: "<<layer1outputs[inputindex2]<<endl;
                bool out=layer2[i]->evaluate();
                layer2outputs.push_back(out);
                cout<<" -> Output: "<<out<<endl;
            }

            cout<<"\n--- Evaluating layer 3 (output gate)---"<<endl;
            outputgate->setinput1(layer2outputs[0]);
            outputgate->setinput2(layer2outputs[1]);
            cout<<"Gate 7 ["<< outputgate->getname() <<"] receives Input A: "<< layer2outputs[0]<<endl;
            cout<<"Gate 7 ["<< outputgate->getname() <<"] receives Input B: "<< layer2outputs[1]<<endl;
            finaloutput=outputgate->evaluate();
        }

        //CKT VISUALIZATION:
        cout << "\n============================================\n";
        cout << "            CIRCUIT VISUALIZATION            \n";
        cout << "============================================\n";
        
        if (totallayer == 1) {
            cout << "Input(s) -> [ " << outputgate->getname() << " ] -> OUTPUT: " << finaloutput << "\n";
        } 
        else if (totallayer == 2) {
            cout << "Input(s) -> [ " << layer1[0]->getname() << " ] --+\n";
            cout << "                      +-> [ " << outputgate->getname() << " ] -> OUTPUT: " << finaloutput << "\n";
            cout << "Input(s) -> [ " << layer1[1]->getname() << " ] --+\n";
        } 
        else if (totallayer == 3) {
            cout << "Input(s) -> [ " << layer1[0]->getname() << " ] --+\n";
            cout << "                      +-> [ " << layer2[0]->getname() << " ] --+\n";
            cout << "Input(s) -> [ " << layer1[1]->getname() << " ] --+             |\n";
            cout << "                                       +-> [ " << outputgate->getname() << " ] -> OUTPUT: " << finaloutput << "\n";
            cout << "Input(s) -> [ " << layer1[2]->getname() << " ] --+             |\n";
            cout << "                      +-> [ " << layer2[1]->getname() << " ] --+\n";
            cout << "Input(s) -> [ " << layer1[3]->getname() << " ] --+\n";
        }
        cout << "============================================\n";
    }
};
//USER INTERFACE AND LOGIC:
class simulator
{
    private:
    gate* creategate(int choice)
    {
        switch(choice)
        {
            case 1: return new ANDGate();
            case 2: return new ORGate();
            case 3: return new NOTGate();
            case 4: return new XORGate();
            case 5: return new XNORGate();
            case 6: return new NANDGate();
            case 7: return new NORGate();
            default: return nullptr;
        }
    }
    gate* createcascadegate(int choice)
    {
        switch(choice)
        {
            case 1: return new ANDGate();
            case 2: return new ORGate();
            case 3: return new XORGate();
            case 4: return new XNORGate();
            case 5: return new NANDGate();
            case 6: return new NORGate();
            default: return nullptr;
        }
    }

    void gatemenu() const
    {
        cout<< "\n Available Gates: "<<endl;
        cout<<"1. AND    2. OR   3. NOT   4. XOR"<<endl;
        cout<<"5. XNOR    6. NAND    7. NOR"<<endl;
    }
    void cascademenu() const
    {
        cout<<endl<<"Availabe Gates for cascading (2-input Gates): "<<endl;
        cout<<"1. AND    2. OR    3. XOR"<<endl;
        cout<<"4. XNOR    5. NAND    6. NOR"<<endl;
    }

    int getvalidvalue( const string& prompt, int minval, int maxval) const 
    {
        int val;
        while(true)
        {
            cout<<prompt;
            cin>>val;
            if(cin.fail()|| val<minval||val>maxval)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout<<"Invalid input! Please enter a number between "<<minval<<"and"<<maxval<<endl;
            }
            else
            {
                return val;
            }
        }
    }

    static int getbinaryinput(const string& prompt)
    {
        int val;
        while(true)
        {
            cout<<prompt;
            cin>>val;
            if(cin.fail()||(val!=0 && val!=1))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout<<"Invalid input! Please enter 0 or 1"<<endl;
            }
            else
            {
                return val;
            }
        }
    }

    gate* selectgate(const string& context)
    {
        cout<<context;
        gatemenu();
        int choice = getvalidvalue("Enter choice(1-7): ", 1,7);
        return creategate(choice);
    }
    gate* selectcascade(const string& context)
    {
        cout<<context;
        cascademenu();
        int choice=getvalidvalue("Enter choice 1-6: ",1,6);
        return createcascadegate(choice);
    }

    public:
    void testsinglegate()
    {
        cout<<endl<<"--- TEST A LOGIC GATE---"<<endl;
        gate* gate=selectgate("");
        cout<<endl<<"---"<<gate->getname()<<"Gate ---"<<endl;
        gate->setinput1(getbinaryinput("Input A (0/1):"));
        if(!gate->setunary())
        {
            gate->setinput2(getbinaryinput("Input B(0/1):"));
        }
        cout<<"Output: "<<gate->evaluate()<<endl;
        delete gate;
    }
    void displaytruthtable()
    {
        cout<<endl<<"---DISPLAY TRUTH TABLE---"<<endl;
        gate* gate=selectgate("");
        cout<<endl<<"Truth table for "<< gate->getname() <<"Gate: "<<endl;
        if(gate->setunary())
        {
            cout<<"A | Output\n";
            cout<<"----------\n";
            for(int i=0;i<=1;i++)
            {
                gate->setinput1(i);
                cout<<" "<< i << " |   "<<gate->evaluate()<<endl;
            }
        }
        else
        {
            cout << " A  B | Output\n";
            cout << "--------------\n";
            for (int i = 0; i <= 1; i++) 
            {
                for (int j = 0; j <= 1; j++) 
                {
                    gate->setinput1(i);
                    gate->setinput2(j);
                    cout << " " << i << "  " << j << " |   " << gate->evaluate() << "\n";
                }
            }
        }
        delete gate;
    }
    void cascadegates()
    {
        cout<<endl<<"--- CASCADE GATES ---"<<endl;
        int numlayers=getvalidvalue("Enter number of layers(1-3): ",1, 3);
        circuit circuit(numlayers);

        if(numlayers==1)
        {
            cout<<endl<<" [LAYER 1: Select Output Gate]"<<endl;
            circuit.setoutputgate(selectcascade("Gate 1: "));
        }

        else if(numlayers==2)
        {
            cout<<endl<<" [LAYER 2: Select 2 Gates]"<<endl;
            for(int i=1;i<=2;i++)
            {
                circuit.addlayer1(selectcascade("Gate " + to_string(i) + ": "));
            }
            cout<<endl<<" [LAYER 2: Select 1 output gate]"<<endl;
            circuit.setoutputgate(selectcascade("Gate 3: "));
        }

        else if(numlayers==3)
        {
            cout<<endl<<" [LAYER 1: Select 4 Gates]"<<endl;
            for(int i=1;i<=4;i++)
            {
                circuit.addlayer1(selectcascade("Gate " + to_string(i) + ": "));
            }
            cout<<endl<<" [LAYER 2: Select 2 Gates]"<<endl;
            for(int i=1;i<=2;i++)
            {
                circuit.addlayer2(selectcascade("Gate " + to_string(i + 4) + ": "));
            }
            cout<<endl<<" [LAYER 3: Select 1 Output Gate]"<<endl;
            circuit.setoutputgate(selectcascade("Gate 7: "));
        }
        circuit.runcircuit(getbinaryinput);
    }

    void start()
    {
        int option=0;
        do 
        {
            cout << "\n╔══════════════════════════════════════╗\n";
            cout << "║        LOGIC GATE SIMULATOR          ║\n";
            cout << "╠══════════════════════════════════════╣\n";
            cout << "║ 1. Test Logic Gate                   ║\n";
            cout << "║ 2. Display Truth Table               ║\n";
            cout << "║ 3. Cascade Gates                     ║\n";
            cout << "║ 4. Exit                              ║\n";
            cout << "╚══════════════════════════════════════╝\n";
            
            option = getvalidvalue("Select an option: ", 1, 4);
            switch(option)
            {
                case 1: testsinglegate(); break;
                case 2: displaytruthtable(); break;
                case 3: cascadegates(); break;
                case 4: cout<<"Exiting Simulator. Goodbye !! \n"; break;
            }
        } while(option !=4);
    }
};

int main()
{
    simulator sim;
    sim.start();
    return 0;
}
