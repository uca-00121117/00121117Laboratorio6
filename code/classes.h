enum lines {NOLINE,SINGLELINE,DOUBLELINE};
enum modes {NOMODE,INT_FLOAT,INT_FLOAT_FLOAT,INT_INT_INT_INT};
enum parameters {THERMAL_CONDUCTIVITY1,THERMAL_CONDUCTIVITY2,HEAT_SOURCE};
enum sizes {NODES,ELEMENTS,DIRICHLET,NEUMANN};

class item{
    protected:
        int id;
        float x;
        float y;
        int node1;
        int node2;
        int node3;
        float value;
    public:
        void setId(int identifier) {
            id = identifier;
        }

        void setX(float x_coord) {
            x = x_coord;
        }

        void setY(float y_coord) {
            y = y_coord;
        }

        void setNode1(int node_1) {
            node1 = node_1;
        }

        void setNode2(int node_2) {
            node2 = node_2;
        }

        void setNode3(int node_3) {
            node3 = node_3;
        }

        void setValue(float value_to_assign) {
            value = value_to_assign;
        }

        int getId() {
            return id;
        }

        float getX() {
            return x;
        }

        float getY() {
            return y;
        }

        int getNode1() {
            return node1;
        }

        int getNode2() {
            return node2;
        }

        int getNode3() {
            return node3;
        }

        float getValue() {
            return value;
        }

        virtual void setValues(int a,float b,float c,int d,int e,int f,float g)=0;

};

class node: public item{

    public:
        void setValues(int a,float b,float c,int d,int e,int f,float g){
            id = a;
            x = b;
            y = c;
        }

};

class element: public item{

    public:
        void setValues(int a,float b,float c,int d,int e,int f,float g){
            id = a;
            node1 = d;
            node2 = e;
            node3 = f;
        }

};

class condition: public item{

    public:

        void setValues(int a,float b,float c,int d,int e,int f,float g){
            node1 = d;
            value = g;
        }

};

class mesh{
        float parameters[3];
        int sizes[4];
        node *node_list;
        element *element_list;
        int *indices_dirich;
        condition *dirichlet_list;
        condition *neumann_list;
    public:
        void setParameters(float E,float A,float f){
            parameters[THERMAL_CONDUCTIVITY1]=E;
            parameters[THERMAL_CONDUCTIVITY2]=A;
            parameters[HEAT_SOURCE]=f;
        }
        void setSizes(int nnodes,int neltos,int ndirich,int nneu){
            sizes[NODES] = nnodes;
            sizes[ELEMENTS] = neltos;
            sizes[DIRICHLET] = ndirich;
            sizes[NEUMANN] = nneu;
        }
        int getSize(int s){
            return sizes[s];
        }
        float getParameter(int p){
            return parameters[p];
        }
        void createData(){
            node_list = new node[sizes[NODES]];
            element_list = new element[sizes[ELEMENTS]];
            indices_dirich = new int[DIRICHLET];
            dirichlet_list = new condition[sizes[DIRICHLET]];
            neumann_list = new condition[sizes[NEUMANN]];
        }
        node* getNodes(){
            return node_list;
        }
        element* getElements(){
            return element_list;
        }
        int* getDirichletIndices(){
            return indices_dirich;
        }
        condition* getDirichlet(){
            return dirichlet_list;
        }
        condition* getNeumann(){
            return neumann_list;
        }
        node getNode(int i){
            return node_list[i];
        }
        element getElement(int i){
            return element_list[i];
        }
        condition getCondition(int i, int type){
            if(type == DIRICHLET) return dirichlet_list[i];
            else return neumann_list[i];
        }
};
