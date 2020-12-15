#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>
using namespace std;

int main()
{
	//GDALAllRegister();
	//GDALDestroyDriverManager(); 

	GDALAllRegister();
	const char* pSrcFilename = "E:\\TEST\\weq.tif";
	const char* pDstFilename = "E:\\TEST\\TEST\\WWGDAL2.tif";
	GDALDataset* poSrcDS = (GDALDataset*)GDALOpen(pSrcFilename, GA_ReadOnly);
	if (poSrcDS == NULL) {
		cout << "Cann't open file " << pSrcFilename << endl;
	}
	GDALRasterBand* poBand = poSrcDS->GetRasterBand(1);
	float* pafScanline = (float*)CPLMalloc(sizeof(float) * 1000 * 1000);
	poBand->RasterIO(GF_Read, 0, 0, 1000, 1000, pafScanline, 1000, 1000, GDT_Float32, 0, 0);
	GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("GTiff"); //HFA for .img
	if (poDriver == NULL) exit(1);
	GDALDataset* poDstDS = poDriver->Create(pDstFilename, 1000, 1000, 1, GDT_Float32, NULL);
	double adfGeoTransform[6];
	if (poSrcDS->GetGeoTransform(adfGeoTransform) == CE_None) {
		poDstDS->SetGeoTransform(adfGeoTransform);
	}
	const char* p_WKT = poSrcDS->GetProjectionRef();
	if (p_WKT != NULL) {
		poDstDS->SetProjection(p_WKT);
	}
	GDALRasterBand* poDstBand = poDstDS->GetRasterBand(1);
	poDstBand->RasterIO(GF_Write, 0, 0, 1000, 1000, (void*)pafScanline, 1000, 1000, GDT_Float32, 0, 0);
	CPLFree(pafScanline);
	GDALClose((GDALDatasetH)poDstDS);
	GDALClose((GDALDatasetH)poSrcDS);

	cout << "gaggasuccess";

	return 0;
}