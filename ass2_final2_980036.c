/* Solution to comp10002 Assignment 2, 2019 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Wen Yee ANG, 980036
   Dated:     13/10/2019

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define DEBUG 0
#define INVALID 0
#define VALID 1
#define ILLEGAL 3
#define ISBLOCK 1
#define NOTBLOCK 0
#define VISITED 1
#define NOTVISITED 0
#define CONTINUE 1
#define END 0
#define NOREPAIR 2
#define PRNTCELL 5
#define NEIGHBOURS 4
#define STARTSIZE 2


/*structure for storing coordinates*/
typedef struct{
	short r;
	short c;
} coord_t;

/*structure for storing all data for grid*/
typedef struct{
	short row_num;
	short column_num;
	short block_num;
	coord_t i_loc; /*stores coordinates of initial cell*/
	coord_t g_loc; /*stores coordinates of goal cel*/
} grid_t;

/* the following code is from listops.c by Dr. Alistair Moffat
with minor alterations*/
typedef struct node node_t;

struct node {
	coord_t loc_data;
	short counter;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;


list_t* make_empty_list(void);
void free_list(list_t *list);
list_t* insert_at_foot(list_t *list, coord_t value);
list_t *insert_at_head(list_t *list, coord_t value, short count);
/*OWN FUNCTION PROTOTYPES*/
char** allocate_grid(grid_t *grid_data);
char** make_empty_grid(grid_t* grid_data, char** grid_visual);
char** fill_grid(char** grid, list_t* route, grid_t* g_data, coord_t* blocks);
void free_grid(char** grid_visual, grid_t* grid_data);
coord_t* get_input(grid_t **g_data, coord_t *blocks, list_t *list, short size);
int check_legal_move(grid_t *grid_data,coord_t *blocks_data,list_t *route_list);
int check_input(grid_t *grid_data, coord_t *blocks_data, list_t *route_list);
int is_block(grid_t *grid_data, coord_t *blocks_data, coord_t route_data);
node_t* blocked_list(grid_t *grid_data,coord_t *blocks_data,list_t *route_list);
int print_0(list_t *route,coord_t *blocks,grid_t *grid,node_t** bked);
void print_2d(char** grid, grid_t *grid_data);
list_t* find_broken(list_t *route_list, node_t* bked_route, list_t *queue_list);
node_t* find_route(grid_t *grid_data, coord_t* blocks_data, node_t* bked_route);
int is_visited(list_t *queue_list, coord_t cell_data);
int is_inbounds(grid_t *grid_data, coord_t cell);
list_t* flood_grid(list_t *queue,node_t* val_cell,grid_t *grid,coord_t *blocks);
list_t* repair_route(list_t *queue_list);
list_t* fuse_route(list_t *route_list, list_t *repair_list, node_t* first_cell);
void print_route(list_t* list);
int get_blocks(grid_t **grid_data, coord_t **blocks_data, short size);
int repairing(node_t** bked_route,coord_t* blocks,grid_t* grid,list_t** route);


int
main(int argc, char *argv[]) {
	int cont=END, repair_status=INVALID;
	short current_size=STARTSIZE, status=INVALID, i=0;
	char c='0';
	char **grid_visual;
	grid_t *grid_data;
	coord_t *blocks_data;
	node_t *bked_route=NULL;
	list_t *route_list;

/*STAGE 0:  ******************************************************************/
	/*allocating space*/
	route_list=make_empty_list();

	blocks_data = (coord_t*)malloc(sizeof(coord_t)*current_size);
	assert(blocks_data);

	grid_data = (grid_t*)malloc(sizeof(grid_t));
	assert(grid_data);

	scanf("%hdx%hd", &grid_data->row_num, &grid_data->column_num);

	/*allocate and initialize visual grid*/
	grid_visual=allocate_grid(grid_data);
	grid_visual=make_empty_grid(grid_data, grid_visual);

	/*get first set of inputs*/
	blocks_data=get_input(&grid_data, blocks_data, route_list, current_size);
	/*print out stage 0*/
	status=print_0(route_list, blocks_data, grid_data, &bked_route);
	/*fill 2D grid visualization with characters*/
	grid_visual=fill_grid(grid_visual, route_list, grid_data, blocks_data);

/*STAGE 1: *******************************************************************/
	printf("==STAGE 1=======================================\n");
	print_2d(grid_visual, grid_data);

	if (!status&&bked_route!=NULL){/*if there is a blocked route*/
		/*attempt first repair*/
		printf("------------------------------------------------\n");

		repair_status=repairing(&bked_route,blocks_data,grid_data,&route_list);

		grid_visual=make_empty_grid(grid_data, grid_visual);
		grid_visual=fill_grid(grid_visual, route_list, grid_data, blocks_data);

		print_2d(grid_visual, grid_data);
		printf("------------------------------------------------\n");

		if (!repair_status){
			printf("The route cannot be repaired!\n");
		}else if((repair_status) && (bked_route!=NULL)){
			print_route(route_list);
			printf("There is a block on this route!\n");
		}else{
			print_route(route_list);
			printf("The route is valid!\n");
		}
	}

	if (status==ILLEGAL){/*if status 1-3, do not proceed to stage 2*/
		cont=END;
		printf("================================================");

	}else if(scanf(" %c ",&c)&&c=='$'){
		printf("==STAGE 2=======================================\n");
		cont++;

	}else{/*if no more input*/
		printf("================================================");
	}

/*continuing to STAGE 2: ****************************************************/
	while (cont){
		repair_status=INVALID;

		free(blocks_data); /*reset blocks data*/
		blocks_data=NULL;
		assert(blocks_data==NULL);

		blocks_data = (coord_t*)malloc(sizeof(coord_t)*current_size);
		assert(blocks_data);

		cont=get_blocks(&grid_data, &blocks_data, STARTSIZE); /*get input*/

		bked_route=blocked_list(grid_data, blocks_data, route_list);

		if (bked_route==NULL){ /*if route is already valid*/
			repair_status=NOREPAIR;
		}

		if (DEBUG){
			printf("Num of blocks=%d\n", grid_data->block_num);
			for (i=0; i<grid_data->block_num; i++){
				printf("Block[%d]=[%d,%d]\n",i,blocks_data[i].r, blocks_data[i].c);
			}
		}

		grid_visual=make_empty_grid(grid_data, grid_visual);
		grid_visual=fill_grid(grid_visual, route_list, grid_data, blocks_data);
		/*print given (not yet repaired) configuration*/
		print_2d(grid_visual, grid_data);

		if (!repair_status){
			printf("------------------------------------------------\n");
		}

		while (bked_route!=NULL){/*repairing the route*/
			repair_status=repairing(&bked_route,blocks_data,grid_data,&route_list);
		}

		grid_visual=make_empty_grid(grid_data, grid_visual);
		grid_visual=fill_grid(grid_visual, route_list, grid_data, blocks_data);

		if (!repair_status){
			print_2d(grid_visual, grid_data);
			printf("------------------------------------------------\n");
			printf("The route cannot be repaired!\n");

		}else if (repair_status==NOREPAIR){
			/*do nothing if route is already valid*/
		}else{
			print_2d(grid_visual, grid_data);
			printf("------------------------------------------------\n");
			print_route(route_list);
			printf("The route is valid!\n");
		}

		printf("================================================");

		if (cont){
			printf("\n");
		}

	}

	/*freeing all used memory****************************************************/
	free_list(route_list);
	route_list=NULL;
	assert (route_list==NULL);

	free_grid(grid_visual, grid_data);
	grid_visual=NULL;
	assert(grid_visual==NULL);

	free(grid_data);
	grid_data=NULL;
	assert(grid_data==NULL);

	if (blocks_data!=NULL){
		free(blocks_data);
		blocks_data=NULL;
		assert (blocks_data==NULL);
	}

	return 0;
}


/*FUNCTIONS*****************************************************************/
int /*repairs broken routes*/
repairing(node_t** bked_route,coord_t* blocks,grid_t* grid,list_t** route){
	list_t *queue_list, *repair_list;
	node_t* first_valid_cell=NULL;

	queue_list=make_empty_list();

	/*find cell before broken segment*/
	queue_list=find_broken(*route, *bked_route, queue_list);
	assert(queue_list);

	/*find first cell after broken segment*/
	first_valid_cell=find_route(grid, blocks, *bked_route);

	if (DEBUG){
		printf("First valid cell is [%d,%d]\n",\
		first_valid_cell->loc_data.r,first_valid_cell->loc_data.c);
	}

	/*flood grid with counters*/
	queue_list=flood_grid(queue_list, first_valid_cell, grid, blocks);

	if (DEBUG){
		if (queue_list==NULL){
			printf("Queue list==NULL, failed\n");
		}else if (queue_list!=NULL){
			printf("queue_list !=NULL\n");
			printf("List flooded. Count=%d\n", queue_list->head->counter);
		}
	}

	*bked_route=NULL;

	if (queue_list!=NULL){/*if the repair did not fail*/
		repair_list=make_empty_list();

		repair_list=repair_route(queue_list);/*make repaired segment*/
		assert(repair_list);

		free_list(queue_list);
		queue_list=NULL;
		assert(queue_list==NULL);

		if (DEBUG){
			printf("This is the repair_list\n");
			print_route(repair_list);
		}

		*route=fuse_route(*route, repair_list, first_valid_cell);
		/*route_list is now the new repaired route*/

		free_list(repair_list);
		repair_list=NULL;
		assert (repair_list==NULL);

		/*check for block on new route*/
		*bked_route=blocked_list(grid, blocks, *route);

	}else if (queue_list==NULL){/*if could not be repaired*/
		*bked_route=NULL;
		return INVALID;
	}

	return VALID;
}


void
print_route(list_t* list) {
	node_t* orig_head=list->head;
	int r=0, c=0, i=0;

	while (list->head->next!=NULL){
		r=list->head->loc_data.r;
		c=list->head->loc_data.c;

		printf("[%d,%d]->", r, c);

		list->head=list->head->next;
		i++;

		if (i==PRNTCELL){
			printf("\n");
			i=0;
		}
	}

	printf("[%d,%d].\n",list->head->loc_data.r,list->head->loc_data.c);

	list->head=orig_head;
}


void
free_grid(char** grid_visual, grid_t* grid_data){
	int a=0;

	if (grid_visual!=NULL){
		for (a=0; a<grid_data->row_num; a++){
			free(grid_visual[a]);
			grid_visual[a]=NULL;
		}
	}

	free(grid_visual);
}


char** /*fill visual grid with characters*/
fill_grid(char** grid, list_t* route, grid_t* g_data, coord_t* blocks){
	int r=0, c=0, i=0;
	node_t* orig_head=route->head;

	while (route->head!=NULL){
		r=route->head->loc_data.r;
		c=route->head->loc_data.c;

		route->head=route->head->next;

		grid[r][c]='*';
	}

	grid[g_data->i_loc.r][g_data->i_loc.c]='I';
	grid[g_data->g_loc.r][g_data->g_loc.c]='G';

	for (i=0; i<g_data->block_num; i++){
		grid[blocks[i].r][blocks[i].c]='#';
	}

	route->head=orig_head;

	return grid;
}


char**
make_empty_grid(grid_t* grid_data, char** grid_visual){
	int r=0,c=0;

	for (r=0;r<grid_data->row_num;r++){

		for (c=0;c<grid_data->column_num;c++){
			grid_visual[r][c]=' ';
		}

	}
	return grid_visual;
}


char** /*allocate memory for visual grid*/
allocate_grid(grid_t *grid_data){
	char** grid_visual;
	int i=0;

	grid_visual = (char**)malloc(sizeof(char*)*grid_data->row_num);
	assert (grid_visual);

	for (i=0;i<grid_data->row_num;i++){
		grid_visual[i] = (char*)malloc(sizeof(char)*grid_data->column_num);
		assert (grid_visual[i]);
	}

	return grid_visual;
}


list_t* /*replaces original route with repaired route segment*/
fuse_route(list_t *route_list, list_t *repair_list, node_t* first_cell){
	node_t *orig_head=route_list->head, *repair_head=repair_list->head;
	list_t* new_route;

	short r=route_list->head->loc_data.r, c=route_list->head->loc_data.c;
	short repair_r_h=repair_list->head->loc_data.r;
	short repair_c_h=repair_list->head->loc_data.c;
	short repair_r_f=repair_list->foot->loc_data.r;
	short repair_c_f=repair_list->foot->loc_data.c;

	new_route=make_empty_list();
	new_route=insert_at_foot(new_route, route_list->head->loc_data);

	while(r!=repair_r_h||c!=repair_c_h){ /*find first repaired cell*/
		route_list->head=route_list->head->next;
		r=route_list->head->loc_data.r;
		c=route_list->head->loc_data.c;
		new_route=insert_at_foot(new_route, route_list->head->loc_data);
	}

	repair_list->head=repair_list->head->next;
	while (repair_list->head!=NULL){
		new_route=insert_at_foot(new_route, repair_list->head->loc_data);
		repair_list->head=repair_list->head->next;
	}

	repair_list->head=repair_head;
	/*skip ahead to after blocked segment*/
	route_list->head=first_cell;

	r=route_list->head->loc_data.r;
	c=route_list->head->loc_data.c;

	while(r!=repair_r_f||c!=repair_c_f){ /*find first repaired cell after broken*/
		route_list->head=route_list->head->next;
		r=route_list->head->loc_data.r;
		c=route_list->head->loc_data.c;
	}

	route_list->head=route_list->head->next;

	while (route_list->head!=NULL){ /*insert end of unbroken route*/
		new_route=insert_at_foot(new_route, route_list->head->loc_data);
		route_list->head=route_list->head->next;
	}

	route_list->head=orig_head;

	free_list(route_list); /*get rid of old list*/
	route_list=NULL;
	assert(route_list==NULL);

	if (DEBUG){
		printf("NEW ROUTE IS\n");
		print_route(new_route);
	}

	return new_route; /*return new list*/
}


list_t* /*finds and returns repaired route segment*/
repair_route(list_t *queue_list){
	coord_t D[NEIGHBOURS]={{-1,0},{1,0},{0,-1},{0,1}};
	short cur_count=queue_list->head->counter;
	short next_count=queue_list->head->next->counter;
	coord_t cur_cell=queue_list->head->loc_data, next_cell, add_cell={0,0};
	list_t *repair_list;
	node_t *cur_head;
	int i=0,found=INVALID;

	repair_list=make_empty_list();
	repair_list=insert_at_head(repair_list, cur_cell, cur_count);

	while(next_count!=0){
		found=INVALID;

		while (next_count!=cur_count-1){
			queue_list->head=queue_list->head->next;
			next_count=queue_list->head->counter;
		}

		cur_head=queue_list->head;
		next_cell=queue_list->head->loc_data;

		for(i=0;i<NEIGHBOURS&&!found;i++){
			add_cell.r=cur_cell.r+D[i].r;
			add_cell.c=cur_cell.c+D[i].c;

			queue_list->head=cur_head;
			next_count=cur_count-1;
			next_cell=cur_head->loc_data;

			while (next_count==cur_count-1&&!found&&next_count!=0){

				if (DEBUG){
					printf("now checking next_cell=[%d,%d], next_count=%d\n"\
					,next_cell.r, next_cell.c, next_count);
				}

				if (add_cell.r==next_cell.r&&add_cell.c==next_cell.c){

					repair_list=insert_at_head(repair_list, next_cell, next_count);

					if (DEBUG){
						printf("insert [%d,%d]==[%d,%d]\n",\
						next_cell.r,next_cell.c,add_cell.r,add_cell.c);
					}

					cur_head=queue_list->head;
					cur_count--;
					cur_cell=next_cell;
					found=VALID;

				}
					queue_list->head=queue_list->head->next;
					next_cell=queue_list->head->loc_data;
					next_count=queue_list->head->counter;
			}

		}/*end of for loop*/
	}/*end of main while loop*/

	repair_list=insert_at_head(repair_list, queue_list->head->loc_data, 0);

	return repair_list;
}


list_t* /*floods grid with pairs and adds to queue*/
flood_grid(list_t *queue,node_t* val_cell,grid_t *grid,coord_t *blocks){
	node_t **cur_cell; /*array to keep track of current cell being processed*/
	int count=1, i=0, j=0, n=0, size=STARTSIZE;
	int valid_r=0, valid_c=0;
	node_t* orig_valid=val_cell;
	coord_t add_cell={0,0};
	coord_t D[NEIGHBOURS]={{-1,0},{1,0},{0,-1},{0,1}};

	cur_cell=(node_t**)malloc(sizeof(node_t*)*size);
	assert(cur_cell);

	cur_cell[j]=queue->head; /*initialize at first cell*/
	int r=cur_cell[j]->loc_data.r, c=cur_cell[j]->loc_data.c;

	assert(queue!=NULL&&cur_cell[j]->counter==0);

	while((j<=n)||j==0){ /*while grid has not been completely flooded*/

		for(i=0;i<NEIGHBOURS;i++){
			/*move to each neighbour*/
			add_cell.r=r+D[i].r;
			add_cell.c=c+D[i].c;

			/*check if within the grid bounds*/
			if(is_inbounds(grid, add_cell)){
				/*check if blocked or visited*/
				if ((!is_block(grid, blocks, add_cell))&&\
				(!is_visited(queue, add_cell))){

					queue=insert_at_head(queue, add_cell, count);/*add to queue*/

					n++;
					cur_cell[n]=queue->head;

					if (DEBUG){
						printf("now at cur_cell[%d]\n",n);
					}

					if (n==size-1){
						size*=2;
						cur_cell=(node_t**)realloc(cur_cell, sizeof(node_t*)*size);
						assert(cur_cell);
					}

					while (val_cell!=NULL){ /*if pair contains a cell in unbroken route*/
						valid_r=val_cell->loc_data.r;
						valid_c=val_cell->loc_data.c;

						if((add_cell.r==valid_r)&&(add_cell.c==valid_c)){
							free(cur_cell);
							cur_cell=NULL;
							assert(cur_cell==NULL);

							if (DEBUG){
								printf("Repair found at [%d,%d]\n",add_cell.r,add_cell.c);
							}

							val_cell=orig_valid;
							/*exit when first unbroken cell on route is found*/
							return queue;
						}

						val_cell=val_cell->next;
					}

					val_cell=orig_valid;
				}
			}
		}/*when all adjacent cells are visited*/

		/*move on to next cell*/
		j++;
		if (DEBUG){
			printf("j=%d, n=%d\n",j,n);
		}

		if (j<=n){
			r=cur_cell[j]->loc_data.r;
			c=cur_cell[j]->loc_data.c;
			count=(cur_cell[j]->counter)+1;
		}

	}
	/*flood failed*/
	free(cur_cell);
	free_list(queue);
	queue=NULL;
	cur_cell=NULL;
	assert(cur_cell==NULL&&queue==NULL);

	val_cell=orig_valid;

	return queue; /*return a NULL pointer to queue if failed*/
}


int/*checks if a cell has already been traversed when flooding grid*/
is_visited(list_t *queue_list, coord_t cell_data) {
	node_t *orig_head=queue_list->head;
	coord_t cur_cell;

	while((queue_list->head)!=NULL){
		cur_cell=queue_list->head->loc_data;

		if (DEBUG){
			printf("checking if visited [%d,%d], count=%d\n",\
			cur_cell.r, cur_cell.c, queue_list->head->counter);
		}

		if((cur_cell.r==cell_data.r)&&(cur_cell.c==cell_data.c)){
			queue_list->head=orig_head;
			return VISITED;
		}

		queue_list->head=queue_list->head->next;
	}

	queue_list->head=orig_head;
	return NOTVISITED;
}


int
is_inbounds(grid_t *grid_data, coord_t cell){
	int r=cell.r, c=cell.c;
	int maxr=grid_data->row_num, maxc=grid_data->column_num;

	if (r>=0&&r<maxr){
		if (c>=0&&c<maxc){
			if (DEBUG){
				printf("Within bounds!\n");
			}
			return VALID;
		}
		return INVALID;
	}

	return INVALID;
}



list_t* /*returns first cell to initialize repair queue*/
find_broken(list_t *route_list, node_t *bked_route, list_t *queue_list) {
	node_t *orig_head=route_list->head;
	coord_t cell;
	int count=0;

	while(route_list->head->next!=bked_route){
		route_list->head=route_list->head->next;
	}

	cell=route_list->head->loc_data; /*first cell before broken segment found!*/

	queue_list=insert_at_head(queue_list, cell, count);

	if (DEBUG){
		printf("Initialize queue at [%d,%d]\n", cell.r, cell.c);
		printf("First block at [%d,%d]\n",\
		bked_route->loc_data.r, bked_route->loc_data.c);
	}

	route_list->head=orig_head;
	return queue_list;
}


node_t* /*finds and returns first non_broken segment*/
find_route(grid_t *grid_data, coord_t *blocks_data, node_t *bked_route){
	node_t *valid_route=bked_route->next;

	while (valid_route!=NULL){
		if (!is_block(grid_data, blocks_data, valid_route->loc_data)){
			return (valid_route);
		}else{
			valid_route=valid_route->next;
		}
	}

	return (valid_route);/*return NULL pointer if there is no valid_route*/
}


coord_t*
get_input(grid_t **g_data, coord_t *blocks, list_t *list, short size){
	g_data[0]->block_num=0;
	short i=0, temp_r=0, temp_c=0;
	coord_t route_loc;

	route_loc.r=route_loc.c=0;

	while (i<2){
		scanf(" [%hd,%hd]", &temp_r, &temp_c);

		if (i==0){ /*initial cell stored*/
			g_data[0]->i_loc.r=temp_r;
			g_data[0]->i_loc.c=temp_c;

		}else if (i==1){ /*goal cell stored*/
			g_data[0]->g_loc.r=temp_r;
			g_data[0]->g_loc.c=temp_c;
		}
		i++;
	}

	get_blocks(g_data, &blocks, size); /*blocks stored*/

	while (scanf("[%hd,%hd]-> ", &route_loc.r, &route_loc.c)==2){
		/*scanning for route*/
		if (DEBUG){
			printf("route: r=%d, c=%d\n",route_loc.r, route_loc.c);
		}

		if (is_inbounds(g_data[0], route_loc)){
			/*only insert route if within grid confines*/
			list=insert_at_foot(list, route_loc);
		}
	}

	return blocks;
}


int
print_0(list_t *route,coord_t *blocks,grid_t *grid,node_t** bked) {
	int r=0, c=0, i=0;
	node_t *orig_head=route->head;

	printf("==STAGE 0=======================================\n");

	printf("The grid has %d rows and %d columns.\n",\
	grid->row_num, grid->column_num);

	printf("The grid has %d block(s).\n", grid->block_num);

	printf("The initial cell in the grid is [%d,%d].\n",\
	grid->i_loc.r,grid->i_loc.c);

	printf("The goal cell in the grid is [%d,%d].\n",\
	grid->g_loc.r,grid->g_loc.c);

	printf("The proposed route in the grid is:\n");

	while (route->head->next != NULL){
		r=route->head->loc_data.r;
		c=route->head->loc_data.c;

		printf("[%d,%d]->", r, c);

		route->head=route->head->next;
		i++;

		if (i==PRNTCELL){
			printf("\n");
			i=0;
		}
	}

	printf("[%d,%d].\n",route->head->loc_data.r,route->head->loc_data.c);

	route->head=orig_head;

	if (check_input(grid, blocks, route)){
		route->head=orig_head;

		/*if input is valid, check for blocked routes*/
		if((*bked=blocked_list(grid, blocks, route))!=NULL){
			route->head=orig_head;
			printf("There is a block on this route!\n");
			if (DEBUG){
				printf("block route at:[%d,%d]\n",\
				bked[0]->loc_data.r, bked[0]->loc_data.c);
			}
			return INVALID;

		}else{
			route->head=orig_head;
			printf("The route is valid!\n");
			return VALID;
		}
	}
	return ILLEGAL;
}


int
check_input(grid_t *grid_data, coord_t *blocks_data, list_t *route_list){
	int initial_r=route_list->head->loc_data.r, initial_c=route_list->head->loc_data.c;
	int goal_r=route_list->foot->loc_data.r, goal_c=route_list->foot->loc_data.c;

	if ((grid_data->i_loc.r!=initial_r)||(grid_data->i_loc.c!=initial_c)){
		/*Report Status 1*/
		printf("Initial cell in the route is wrong!\n");
		return INVALID;

	}else if((grid_data->g_loc.r!=goal_r)||(grid_data->g_loc.c!=goal_c)){
		/*Report Status 2*/
		printf("Goal cell in the route is wrong!\n");
		return INVALID;

	}else if(!(check_legal_move(grid_data, blocks_data, route_list))){
		/*Report Status 3*/
		return INVALID;
	}

	return VALID;
}


int
check_legal_move(grid_t *grid_data, coord_t *blocks_data, list_t *route_list) {
	int cur_r=0, cur_c=0, next_r=0, next_c=0;

	while (route_list->head->next!=NULL){
		cur_r=route_list->head->loc_data.r;
		cur_c=route_list->head->loc_data.c;

		next_r=route_list->head->next->loc_data.r;
		next_c=route_list->head->next->loc_data.c;

		if (DEBUG){
			printf("compare [%d,%d]and[%d,%d]\n",cur_r,cur_c,next_r,next_c);
		}

		if(cur_r==next_r&&cur_c==next_c){ /*stay on same cell*/
			printf("There is an illegal move in this route!\n");
			if (DEBUG){
				printf("not moving\n");
			}
			return INVALID;

		}else if (cur_r!=next_r&&cur_c!=next_c){ /*moving diagonally*/
			printf("There is an illegal move in this route!\n");
			if (DEBUG){
				printf("diagonal\n");
			}
			return INVALID;

		}else if(cur_r!=next_r&&((cur_r-next_r)!=1&&(cur_r-next_r)!=-1)){
			/*move more than one cell*/
			printf("There is an illegal move in this route!\n");
			if (DEBUG){
				printf("not one cell move r\n");
			}
			return INVALID;

		}else if(cur_c!=next_c&&((cur_c-next_c)!=1&&(cur_c-next_c)!=-1)){
			/*move more than one cell*/
			printf("There is an illegal move in this route!\n");
			if (DEBUG){
				printf("not one cell move c\n");
			}
			return INVALID;
		}
		route_list->head=route_list->head->next;
	}
	return VALID;
}


int
get_blocks(grid_t **grid_data, coord_t **blocks_data, short size){
	grid_data[0]->block_num=0;
	short b=0, temp_r=0, temp_c=0;
	char c='0';

	while (scanf(" [%hd,%hd]", &temp_r, &temp_c)==2){
		blocks_data[0][b].r=temp_r;
		blocks_data[0][b].c=temp_c;

		if (DEBUG){
			printf("block_num=%d, current_size=%d", b, size);
			printf("block[%d,%d]\n", blocks_data[0][b].r, blocks_data[0][b].c);
		}

		if (b==size-1){
			size*=2;
			*blocks_data = realloc(*blocks_data, sizeof(coord_t)*size);
			if (DEBUG){
				printf("Allocate successful!\n");
			}
			assert(*blocks_data);
		}

		b++;
	}

	grid_data[0]->block_num=b;

	if(scanf(" %c ",&c)&&c=='$'){
		if (DEBUG){
			printf("%c\n",c);
		}
		return CONTINUE;
	}
	return END;
}


int
is_block(grid_t *grid_data, coord_t *blocks_data, coord_t route_data) {
	int i=0;
	for (i=0;i<(grid_data->block_num);i++){
		if ((route_data.r==blocks_data[i].r)&&(route_data.c==blocks_data[i].c)){
			return ISBLOCK;
		}
	}
	return NOTBLOCK;
}


node_t*
blocked_list(grid_t *grid_data, coord_t *blocks_data, list_t *route_list){
	coord_t cur_data;
	node_t* bked_route=NULL;
	node_t* orig_head=route_list->head;

	while (route_list->head->next!=NULL){
		cur_data=route_list->head->loc_data;

		if(is_block(grid_data, blocks_data, cur_data)){
			bked_route=route_list->head;
			if (DEBUG){
				printf("Blocked route found at [%d,%d]\n",\
				bked_route->loc_data.r, bked_route->loc_data.c);
			}
			route_list->head=orig_head;
			return bked_route;
		}

		route_list->head=route_list->head->next;
	}

	route_list->head=orig_head;

	return bked_route;
}


void
print_2d(char** grid, grid_t *grid_data) {
	int n=0, r=0, c=0, i=0;
	printf(" ");

	for (i=0; i<grid_data->column_num; i++){
		if (n>9){
			n=0;
		}
		printf("%d",n);
		n++;
	}

	printf("\n");
	n=0;

	for(r=0; r<grid_data->row_num; r++){
		if(n>9){
			n=0;
		}
		printf("%d",n);
		for (c=0; c<grid_data->column_num; c++){
			printf("%c", grid[r][c]);
		}
		printf("\n");
		n++;
	}

	return;
}


/*The following code is from listops.c by Dr. Alistair Moffat with
minor alterations*/
list_t
*insert_at_foot(list_t *list, coord_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->counter = 0;
	new->loc_data = value;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

list_t
*insert_at_head(list_t *list, coord_t value, short count) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	if (DEBUG){
		printf("Insert head successful!\n");
	}
	new->loc_data = value;
	new->counter = count;
	new->next = list->head;
	list->head = new;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->foot = new;
	}
	return list;
}

void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
	if (DEBUG){
		printf("Free list successful\n");
	}
}

list_t
*make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

/*ALGORITHMS ARE FUN!!!*/
