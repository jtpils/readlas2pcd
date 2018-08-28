#include <iostream>
#include <string>
#include <vector>

#include <pcl/console/parse.h>

//#include "readlas2pcd.cpp"
#include "convertlas.h"

typedef pcl::PointXYZI PointT;

using namespace std;


///////////////////////////////////////////////////////////////////////////////////////
template <typename PointInT>
void
viewer (typename pcl::PointCloud<PointInT>::Ptr &cloud)
{

  pcl::visualization::CloudViewer viewer ("View Point cloud");
  viewer.showCloud (cloud);
  while (!viewer.wasStopped ())
  {
  }
}

///////////////////////////////////////////////////////////////////////////////////////

int  main (int argc, char** argv){

// Checking parameters and input arguments
   if (argc == 1){
        PCL_ERROR ("No file specified... aborting.");
        return -1;
    }


  pcl::PointCloud<PointT>::Ptr cloudIn (new pcl::PointCloud<PointT>);


// Checking parameters and input arguments
  if (argc < 2){
      pcl::console::print_error ("Syntax is: %%s <LAS-INPUT>  \t are mandatory.\nOtional are: \n "
                                         "-setZero: setMinXYZ close to zero. set to 1 (enable) or 0 (disable) \n "
                                         "-downsample: float [0, 0.025,1] \n"
                                         "-ext: (optional), use [pcd=default or txt]", argv[0]);
      return -1;
   }

    // check optional parameters for subtracting the min Values of the coordinates from all x,y,z coordinates respectively
    // this is done to work around problems caused by the loss of precision using float data types. -> poor PCD point clouds.
    bool subtractMinVals = false;
    pcl::console::parse (argc, argv, "-setZero", subtractMinVals);

    //check if extension keyword was specified. If not, use pcd, else use xyz extension.
    bool Extension_specified = pcl::console::find_switch (argc, argv, "-ext");
    std::string ext = "txt";
    if (Extension_specified){
        pcl::console::parse (argc, argv, "-ext", ext);
        if (ext.compare("pcd") != 0 || ext.compare("pcd") != 0 || ext.compare("pcd") != 0 )
            std::cout << "debug: extension is: " << ext << std::endl;
        else {
            std::cout << "unknown extension: " << ext << "\nUsing default extension txt" << std::endl;
            ext = "txt";
        }
    }

    // check if the user wants to downsample the point cloud using the voxelgrid function. Specfiy standard values for the leafSize.
    float gridLeafSize = 0.25;
    bool LeafSize_specified = pcl::console::find_switch (argc, argv, "-downsample");
    if (!LeafSize_specified)
        gridLeafSize = 0;

    if (LeafSize_specified){
        pcl::console::parse (argc, argv, "-downsample", gridLeafSize);
        if (gridLeafSize > 1){
            pcl::console::print_highlight("Warning: LeafSize is out of bounds [0 or between 0.025, 1]. Setting it to default to proceed (0.25).\n");
            gridLeafSize = 0.25;
        }
        else if (gridLeafSize < 0.025 && gridLeafSize != 0){
            pcl::console::print_highlight("Warning: LeafSize is out of bounds [0 or between 0.025, 1]. Setting it to default to proceed (0.25).\n");
            gridLeafSize = 0.25;
        }
        else if (gridLeafSize == 0){
             pcl::console::print_highlight("downsampling disabled\n");
              gridLeafSize = 0;
        }
    }


           // store minvalues to restore the original coordinates later after processing is completed
        std::vector<double> minXYZValues;
        // reading in LAS file and returning a PCD cloud PointT data.
       // if (ext.compare("pcd") == 0){
        convertLAS<PointT>(argv[1], cloudIn, minXYZValues, gridLeafSize, subtractMinVals, ext);
        //    viewer<PointT> (cloudIn);
       // }
        //else
         //   readlas2txt<PointT>(argv[1]);

    return (0);
}
