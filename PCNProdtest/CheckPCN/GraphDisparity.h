#pragma once
#include "../Structures.h"
#include "../utils.h"
#include "../define_file.h"
#include "connected.h"
#include <assert.h>

using namespace std;

#define SIZE_BORDER_CHECK 7
static int  MIN_NUM_PIXEL_PER_NODE_BORDER = 70;
static int MIN_NUM_PIXEL_PER_NODE = 30;
static int MIN_NUM_PIXEL_FOR_CENTROID_CHECK = 400;
static int MAX_ROW_DELTA = 25;


class GraphDisparity
{
public:
  GraphDisparity(void);
  ~GraphDisparity(void);

  int getNNodes();


  PixelType* printNodes();
	PixelType* printResults(vector<tResultsCheckGraph> res);

  vector<tResultsCheckGraph> check(PixelType* img, const int& w, const int& h);

  static void setParameters(tParametersCheck param);
private:
  void addPixel(PixelType value, int label, const int& pos);
  vector<tResultsCheckGraph> checkGraph();

  void calcNear();
  int indexNode( int label );
  void addNode( PixelType value, int label, const int& pos );
  void updateNode( const int& index, const int& pos );
  void getNeighborhood(const int& pos, vector<int>& positions, bool* img_flags);
  void updateNearPositions(const int& index, const int& near_pos);
  int findNodeFromPos( const int& pos );
  bool isNear( const int& index, const int& near_index );
  std::pair<int, int> getCoordToPos( const int& pos );
  bool isCoordNear(const int& first, const int& second);

  // Check
  void nodePruning();
  tResultsCheckGraph checkWeightEdge();
  void pruningEdge(const int& index_nodes);
  tResultsCheckGraph checkInternalBlob();
	tResultsCheckGraph checkBorderDisparity();

  tResultsCheckGraph checkCentroid();
  void computeCentroid(tNode& node,int& y_c, int& x_c);
  bool isNotBorderNodes( const int& index );
  tResultsCheckGraph checkPath();
  bool check_path(int& index );
  bool isNodeBorderFirst( const int& index );
  bool isNodeBorderEnd( const int& index );
  tResultsCheckGraph checkNumDisparity();
  bool justFound(const int& disp_value,vector<int> & disparity);
  tResultsCheckGraph checkShape();
  void findlimits(tNode& node,
    int& x_l,
    int& y_l_max,
    int& y_l_min,
    int& x_r,
    int& y_r_max,
    int& y_r_min);
  void clearForCheck();
	void addPosToVect(vector<int>& out, const vector<int>& in);
private:
  vector<tNode> nodes;
  // Indici dei nodi ai bordi
  vector<int> bordersFirst;
  vector<int> bordersEnd;
};
