/*
   Source File : TestMeasurementsTest.cpp


   Copyright 2011 Gal Kahana PDFWriter

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   
*/

#include "PDFWriter.h"
#include "PDFPage.h"
#include "PageContentContext.h"

#include "testing/TestIO.h"

#include <iostream>

using namespace PDFHummus;

int HighLevelImages(int argc, char* argv[])
{
	EStatusCode status = eSuccess;
	PDFWriter pdfWriter;

	do
	{
		status = pdfWriter.StartPDF(BuildRelativeOutputPath(argv,"HighLevelImages.pdf"),
                                    ePDFVersion13,
                                    LogConfiguration(true,true,
                                                     BuildRelativeOutputPath(argv,"HighLevelImages.log")));
		if(status != eSuccess)
		{
			cout<<"Failed to start file\n";
			break;
		}

		PDFPage* page = new PDFPage();
		page->SetMediaBox(PDFRectangle(0,0,595,842));
		
		PageContentContext* cxt = pdfWriter.StartPageContentContext(page);

		// simple image placement
		cxt->DrawImage(10,10,BuildRelativeInputPath(argv,"images/soundcloud_logo.jpg"));
#ifndef PDFHUMMUS_NO_TIFF
		cxt->DrawImage(10,500,BuildRelativeInputPath(argv,"images/tiff/cramps.tif"));
#endif // PDFHUMMUS_NO_TIFF
		cxt->DrawImage(0,0,BuildRelativeInputPath(argv,"XObjectContent.pdf"));

		status = pdfWriter.EndPageContentContext(cxt);
		if(status != eSuccess)
		{
			status = PDFHummus::eFailure;
			cout<<"Failed to end content context\n";
			break;
		}

		status = pdfWriter.WritePageAndRelease(page);
		if(status != eSuccess)
		{
			status = PDFHummus::eFailure;
			cout<<"Failed to write page\n";
			break;
		}

		page = new PDFPage();
		page->SetMediaBox(PDFRectangle(0,0,595,842));
		cxt = pdfWriter.StartPageContentContext(page);

		AbstractContentContext::ImageOptions opt1;
		opt1.imageIndex = 2;
#ifndef PDFHUMMUS_NO_TIFF
		cxt->DrawImage(10,10,BuildRelativeInputPath(argv,"images/tiff/multipage.tif"),opt1);
#endif // PDFHUMMUS_NO_TIFF
		AbstractContentContext::ImageOptions opt2;
		opt2.transformationMethod = AbstractContentContext::eMatrix;
		opt2.matrix[0] = opt2.matrix[3] = 0.25;
		cxt->DrawImage(10,10,BuildRelativeInputPath(argv,"images/soundcloud_logo.jpg"),opt2);
		AbstractContentContext::ImageOptions opt3;
		opt3.transformationMethod = AbstractContentContext::eFit;
		opt3.boundingBoxHeight = 100;
		opt3.boundingBoxWidth = 100;
		cxt->DrawImage(0,0,BuildRelativeInputPath(argv,"XObjectContent.pdf"),opt3);
		opt3.fitProportional = true;
		cxt->DrawImage(100,100,BuildRelativeInputPath(argv,"XObjectContent.pdf"),opt3);

		// draw frames for soundcloud_logo.jpg and the 2 fitted version of XObjectContent.pdf
		AbstractContentContext::GraphicOptions pathStrokeOptions(AbstractContentContext::eStroke,
																	AbstractContentContext::eRGB,
																	AbstractContentContext::ColorValueForName("DarkMagenta"),
																	4);
		cxt->DrawRectangle(0,0,100,100,pathStrokeOptions);
		cxt->DrawRectangle(100,100,100,100,pathStrokeOptions);

		DoubleAndDoublePair jpgDimensions = pdfWriter.GetImageDimensions(BuildRelativeInputPath(argv,"images/soundcloud_logo.jpg"));
		cxt->DrawRectangle(10,10,jpgDimensions.first/4,jpgDimensions.second/4,pathStrokeOptions);



		status = pdfWriter.EndPageContentContext(cxt);
		if(status != eSuccess)
		{
			status = PDFHummus::eFailure;
			cout<<"Failed to end content context\n";
			break;
		}

		status = pdfWriter.WritePageAndRelease(page);
		if(status != eSuccess)
		{
			status = PDFHummus::eFailure;
			cout<<"Failed to write page\n";
			break;
		}


		status = pdfWriter.EndPDF();
		if(status != eSuccess)
		{
			status = PDFHummus::eFailure;
			cout<<"Failed to end pdf\n";
			break;
		}

	}while(false);


	return status;
}

