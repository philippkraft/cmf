

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#ifndef RasterTemplate_h__
#define RasterTemplate_h__



#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#endif
namespace raster { 
	/// Holds the statistics for a raster
	struct RasterStatistics
	{			
		double 
			min, ///< Minimum value in raster
			max, ///< Maximum value in raster
			mean,///< Mean value in raster
			stdev; /// Standard deviation of raster
		int count; /// Count of cells with data
		RasterStatistics() : min(0),max(0),mean(0),stdev(0),count(0) {}
		// 			RasterStatistics(const RasterStatistics& rs) : min(rs.min),max(rs.max),mean(rs.mean),stdev(rs.stdev),count(rs.count) {}
		// 			RasterStatistics& operator=(const RasterStatistics& rs)
		// 			{
		// 				min=rs.min;max=rs.max;mean=rs.mean;stdev=rs.stdev;count=rs.count;
		// 			}
	};
	/// Holds the histogram for a raster
	class Histogram
	{
	private:
		double m_width,m_min;
		int count;
		std::vector<int> m_frequency;
	public:
		/// Returns the left side of the first histogram bar
		double min() const  { return m_min;}
		/// Returns the right side of the last histogram bar
		double max()  const { return m_min+m_width*m_frequency.size();}
		/// Returns the width of the histogram bars
		double barwidth() const { return m_width;} 
		/// Returns the number of the historam bars
		int size() const   { return int(m_frequency.size());}
		/// Returns the number of entries with a value <= until, default is the total number of entries
		int sum(double until=1e308)	const
		{
			if (until>max()) return count;
			int s=0,p=pos(until);
			for (int i = 0; i < size() && i<p ; ++i)
				s+=frequency(i);
			return s;
		}
		/// Returns the frequency of values near val
		int frequency(double val) const
		{
			int p=pos(val);
			if (p>=0 && p<size())
				return m_frequency[p];
			else
				return 0;
		}
		/// Returns the relative frequency of values near val
		double relfrequency(double val) const
		{
			return double(frequency(val))/double(sum());
		}
		/// Returns the frequency of bar number pos
		int frequency(int pos) const
		{
			return m_frequency.at(pos);
		}
		/// Returns the relative frequency of bar number pos
		double relfrequency(int pos) const
		{
			return double(frequency(pos))/double(sum());
		}
		/// Returns the bar number, that contains value val
		int pos(double val) const { 
			if (val>max()) return size();
			else if (val<min()) return -1;
			else return int((val-m_min)/m_width);
		}
		/// Returns the center of the bar at position pos
		double barcenter(double pos) const {
			return (pos+0.5)*m_width+m_min;
		}

		/// Returns the inexact quantile. The result gets better with higher numbers of bars
		/// Assumes the values of each bar uniformly distributed
		double quantile(double Quantile=0.5) const
		{
			int
				// Number of items in quantile
				max_count = int(count*Quantile+0.5),
				// Item counter
				cnt=0,
				// bar counter
				i=0,
				// item counter for (i-1)
				lastcnt=0;
			// If Quantile > 1 return max()
			if (max_count>=count) return max();
			// If Quantile < 0 return min()
			else if (max_count<=0) return min();
			else
			{
				while (cnt<max_count)
				{
					lastcnt=cnt;
					cnt+=m_frequency[i++];
				}
				if (frequency(i))
					return (i-1)*m_width+m_min + double(max_count-lastcnt)/double(frequency(i))*m_width;
				else
					while (!frequency(i))
						--i;
				return i*m_width+m_min;

			}
		}
		/// Counts a value in the histogram
		void CountValue(double val)
		{
			int p=pos(val);
			p=p<0 ? 0 : p>=size() ? size()-1 : p;
			++m_frequency[p];
			++count;
		}
		/// Creates a histogram with bars ranging from _min to _max with a width of width
		Histogram(double _min,double _max, double _width) : m_min(_min),m_width(_width),m_frequency(size_t((_max-_min)/_width),0),count(0) {}
		/// Creates a histogram with count bars ranging from _min to _max
		Histogram(double _min,double _max, size_t _count) : m_min(_min),m_width((_max-_min)/_count),m_frequency(_count,0),count(0) {}


	};
	///Represents a raster dataset.
	template<class rasterType> 
	class Raster
	{
		RasterStatistics m_statistic;
		bool m_statistic_actual;
#ifndef SWIG
		/// Definition of the spatial properties of the raster
		struct header {
		public:
			double      xllcorner; ///<x-Value for the Lower Left corner
			double      yllcorner; ///<y-Value for the Lower Left corner
			double      Xcellsize; ///<Cellsize on X-Axis of each cell
			double			Ycellsize; ///<Cellsize on Y-Axis of each cell
			int         ncols;		 ///<Number of columns in the raster
			int         nrows;		 ///<Number of rows in the raster
			rasterType  NoData;    ///<NO DATA or NULL - Value, often -9999

			header() {}
			/// Creates a new header with given values
			header(int _ncols,int _nrows, double _xllcorner, double _yllcorner, double _xcellsize,double _ycellsize,rasterType nodata=rasterType(-9999))
			{
				xllcorner=_xllcorner;
				yllcorner=_yllcorner;
				ncols=_ncols;
				nrows=_nrows;
				Xcellsize=_xcellsize;
				Ycellsize=_ycellsize;
				NoData=nodata;
			}

			/// Gets a header in the ESRI-Format (either beginning of .asc-file or a .hdr-file)
			header(std::istream& ASCFile)
			{
				std::string dummy(50,0);
				double shiftFactor=0;

				ASCFile >> dummy >> ncols;
				ASCFile >> dummy >> nrows;
				ASCFile >> dummy >> xllcorner;
				if (dummy.find("center")) shiftFactor=0.5;
				ASCFile >> dummy >> yllcorner;
				ASCFile >> dummy >> Xcellsize;
				if (dummy.find("X")!=std::string::npos || dummy.find("x")!=std::string::npos)
					ASCFile >> dummy >> Ycellsize;
				else
					this->Ycellsize=Xcellsize;
				ASCFile >> dummy >> NoData;
				//Corrects the corner position if the center of the lower left cell is given
				xllcorner-=Xcellsize*shiftFactor;
				yllcorner-=Ycellsize*shiftFactor;
			}
			/// Writes the header to a stream
			void WriteToStream( std::ostream& ASCFile ) const 
			{
				ASCFile << "ncols " << ncols << std::endl;
				ASCFile << "nrows " << nrows << std::endl;
				ASCFile << "xllcorner " << xllcorner << std::endl;
				ASCFile << "yllcorner " << yllcorner << std::endl;
				if (Xcellsize != Ycellsize) 
				{
					ASCFile << "xcellsize" << Xcellsize << std::endl;
					ASCFile << "ycellsize" << Ycellsize << std::endl;
				}
				else
					ASCFile << "cellsize " << Xcellsize << std::endl;
				ASCFile << "NODATA_value " << NoData << std::endl;
			}

		};
		header m_Header; ///<contains the header of the dataset
		typedef std::vector<rasterType> data;
		data m_data; ///<The dataset

		/// Loads the data from an ESRI ASC-File (without the header)
		void LoadFromASC(std::istream& ASCFile)
		{
			m_data=std::vector<rasterType>(m_Header.ncols*m_Header.nrows);
			data::iterator pdata=m_data.begin();
			double buffer=0;
			for (int i=0 ;i<m_Header.ncols*m_Header.nrows;i++)
			{
				if (ASCFile.eof()) 
					throw std::istream::failure("Unexpected end of ASCII-Rasterstream");
				ASCFile >> buffer;
				*pdata++ = rasterType(buffer);
			}
		}

#endif
	public:
		/// @name Metadata
		//@{
		///x-Coordinate for the Lower Left corner (in cellsize units)
		double Xllcorner() const {return m_Header.xllcorner;}
		///y-Coordinate for the Lower Left corner (in cellsize units)
		double Yllcorner() const {return m_Header.yllcorner;}
		///Cellsize of the raster
		double XCellsize() const {return m_Header.Xcellsize;}
		///Cellsize of the raster
		double YCellsize() const {return m_Header.Ycellsize;}
		///Number of columns in the raster
		int ColumnCount() const {return m_Header.ncols;}
		///Number of rows in the raster
		int    RowCount() const {return m_Header.nrows;}
		///Extent W-E in cellsize units
		double Width() const {return m_Header.ncols*m_Header.Xcellsize;}
		///Extent N-S in cellsize units
		double Height() const {return m_Header.nrows*m_Header.Ycellsize;}
		///Returns rasterType (NoData)
		rasterType NoData() const { return m_Header.NoData; }
		/// Returns the bounding box of the raster
		//@}
		/// @name Data access
		//@{
		///Returns the value of the raster at the given position.
		rasterType GetData(double x, double y) const {
			return this->GetData(
				(int)((x-m_Header.xllcorner)/m_Header.Xcellsize),
				m_Header.nrows-(int)((y-m_Header.yllcorner)/m_Header.Ycellsize)-1
				);}

		///Returns the value of the raster at the given cell.
		rasterType GetData(int col, int row) const
		{
			if (HasData(col,row))
				return m_data[col+m_Header.ncols*row];
			else 
				return m_Header.NoData;
		}
		bool HasData(double x,double y) const {
			return HasData(
				(int)((x-m_Header.xllcorner)/m_Header.Xcellsize),
				m_Header.nrows-(int)((y-m_Header.yllcorner)/m_Header.Ycellsize)-1);
		}

		///Returns true if there is data at the given position
		bool HasData(int col, int row) const {
			return	(
				col>=0 && col<m_Header.ncols && 
				row>=0 && row<m_Header.nrows && 
				m_data[col+m_Header.ncols*row]!=m_Header.NoData
				);
		}
		Raster<int> HasData() const {
			Raster<int> res(m_Header.ncols,m_Header.nrows,m_Header.xllcorner,m_Header.yllcorner,m_Header.Xcellsize,m_Header.Ycellsize,-9999);
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					res.SetData(c,r,int(this->HasData(c,r)));
				}
			return res;
		}
		///Sets a value at the given position
		/// @param x X-coordinate
		/// @param y Y-coordinate
		/// @param val Value to set at (x,y)
		void SetData(double x,double y,rasterType val)
		{
			SetData((int)((x-m_Header.xllcorner)/m_Header.Xcellsize),
				m_Header.nrows-(int)((y-m_Header.yllcorner)/m_Header.Ycellsize)-1,
				val);
		}
		///Sets a value at the given position
		/// @param col Column of the raster
		/// @param row Row of the raster
		/// @param val Value to set at (col,row)
		void SetData(int col,int row, rasterType val)
		{
			if (col>=0 && col<m_Header.ncols && row>=0 && row<m_Header.nrows)
			{
				m_data[col+m_Header.ncols*row]=val;
				m_statistic_actual=false;
			}
		}
		//@}

		/// Returns the real world x position of a column
		double GetXPosition(int col)			{
			return (col+0.5)*m_Header.Xcellsize+m_Header.xllcorner;			}
		/// Returns the real world y position of a row
		double GetYPosition(int row)			{
			return (m_Header.nrows-row-0.5)*m_Header.Ycellsize+m_Header.yllcorner;			}
		/// @name Analysis
		//@{
		/// Creates statistics for the raster
		RasterStatistics statistics()
		{
			if (m_statistic_actual) return m_statistic;
			m_statistic=RasterStatistics();
			m_statistic.min=double(NoData());
			m_statistic.max=double(NoData());
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					if (this->HasData(c,r))
					{
						rasterType val=GetData(c,r);
						m_statistic.min=m_statistic.min==double(NoData()) || val<m_statistic.min ?  double(val) : m_statistic.min;
						m_statistic.max=m_statistic.max==double(NoData()) || val>m_statistic.max ?  double(val) : m_statistic.max;
						m_statistic.mean+=val;
						m_statistic.stdev+=val*val;				
						++m_statistic.count;
					}	
				}
				m_statistic.mean/=m_statistic.count;
				m_statistic.stdev=sqrt(m_statistic.stdev/m_statistic.count - m_statistic.mean*m_statistic.mean);
				return m_statistic;
		}
		Histogram histogram(size_t bins=100) 
		{
			RasterStatistics stat=statistics();
			Histogram hist(stat.min,stat.max,bins);
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
					if (this->HasData(c,r))
						hist.CountValue(this->GetData(c,r));
			return hist;
		}
		//@}
		/// @name Operators
		//@{
		/// Mulitplies all the values with a scalar
		Raster<rasterType>& operator*=(rasterType scalar)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					if (HasData(c,r))
						m_data[r*m_Header.ncols+c]*=scalar;
				}
				return *this;
		}
		/// Multiplies all the values with the value at the same location in the given raster
		Raster<rasterType>& operator*=(const Raster<rasterType>& raster)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					double 
						x=GetXPosition(c),
						y=GetYPosition(r);
					if (HasData(c,r) && raster.HasData(x,y))
						m_data[r*m_Header.ncols+c]*=raster.GetData(x,y);
					else
						m_data[r*m_Header.ncols+c]=this->NoData();
				}
				return *this;
		}
		/// Add to all the values a scalar
		Raster<rasterType>& operator+=(rasterType scalar)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{

					if (HasData(c,r))
						m_data[r*m_Header.ncols+c]+=scalar;
				}
				return *this;
		}
		/// Add to all the values the value at the same location in the given raster
		Raster<rasterType>& operator+=(const Raster<rasterType>& raster)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					double 
						x=GetXPosition(c),
						y=GetYPosition(r);
					if (HasData(c,r) && raster.HasData(x,y))
						m_data[r*m_Header.ncols+c]+=raster.GetData(x,y);
					else
						m_data[r*m_Header.ncols+c]=NoData();
				}
				return *this;
		}
		/// Subtract from all the values a scalar
		Raster<rasterType>& operator-=(rasterType scalar)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{

					if (HasData(c,r))
						m_data[r*m_Header.ncols+c]-=scalar;
				}
				return *this;
		}
		/// Subtract from all the values the value at the same location in the given raster
		Raster<rasterType>& operator-=(const Raster<rasterType>& raster)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					double 
						x=GetXPosition(c),
						y=GetYPosition(r);
					if (HasData(c,r) && raster.HasData(x,y))
						m_data[r*m_Header.ncols+c]-=raster.GetData(x,y);
					else
						m_data[r*m_Header.ncols+c]=NoData();

				}
				return *this;
		}
		Raster<rasterType>& operator/=(rasterType scalar)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{

					if (HasData(c,r))
						m_data[r*m_Header.ncols+c]/=scalar;
				}
				return *this;
		}
		/// Subtract from all the values the value at the same location in the given raster
		Raster<rasterType>& operator/=(const Raster<rasterType>& raster)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					double 
						x=GetXPosition(c),
						y=GetYPosition(r);
					if (HasData(c,r) && raster.HasData(x,y))
					{
						rasterType v=raster.GetData(x,y);
						m_data[r*m_Header.ncols+c]/= v!=0 ? v : NoData();
					}
					else
						m_data[r*m_Header.ncols+c]=NoData();

				}
				return *this;
		}

		Raster<rasterType> operator*(rasterType scalar)
		{
			Raster<rasterType> res(*this);
			res*=scalar;
			return res;
		}
		Raster<rasterType> operator+(rasterType scalar)
		{
			Raster<rasterType> res(*this);
			res+=scalar;
			return res;
		}
		Raster<rasterType> operator-(rasterType scalar)
		{
			Raster<rasterType> res(*this);
			res+=scalar;
			return res;
		}
		Raster<rasterType> operator/(rasterType scalar)
		{
			Raster<rasterType> res(*this);
			res/=scalar;
			return res;
		}
		Raster<rasterType> operator*(const Raster<rasterType> & other)
		{
			Raster<rasterType> res(*this);
			res*=other;
			return res;
		}
		Raster<rasterType> operator+(const Raster<rasterType> & other)
		{
			Raster<rasterType> res(*this);
			res+=other;
			return res;
		}
		Raster<rasterType> operator-(const Raster<rasterType> & other)
		{
			Raster<rasterType> res(*this);
			res-=other;
			return res;
		}
		Raster<rasterType> operator/(const Raster<rasterType> & other)
		{
			Raster<rasterType> res(*this);
			res/=other;
			return res;
		}
		//@}

		/// @name Constructors & IO-Methods
		//@{
		///Creates an empty Raster dataset	
		Raster(int ncols,int nrows, double xllcorner, double yllcorner, double xcellsize,double ycellsize, rasterType nodata,rasterType initialValue=0)
			: m_Header(ncols,nrows,xllcorner,yllcorner,xcellsize,ycellsize,nodata),
			m_data(ncols*nrows,nodata),
			m_statistic_actual(false),m_statistic()
		{	}
		/// Copy constructor
		Raster(const Raster<rasterType>& R) 
			: 
		m_Header(R.ColumnCount(),R.RowCount(),R.Xllcorner(),R.Yllcorner(),R.XCellsize(),R.YCellsize(),R.NoData()),
			m_data(R.m_data),m_statistic_actual(false),m_statistic()
		{ }
		/// Copy constructor, creates an empty raster dataset with the same spatial properties like the input raster
		Raster(const Raster<rasterType>& R,rasterType FixedValue) 
			: 
		m_Header(R.ColumnCount(),R.RowCount(),R.Xllcorner(),R.Yllcorner(),R.XCellsize(),R.YCellsize(),R.NoData()),
			m_data(R.ColumnCount()*R.RowCount(),FixedValue),m_statistic_actual(false)
		{			}
		///Builds a new Rasterdataset and passes the ownership of the dataset to the Raster. No external reference to the dataset should be used.
		/// Loads an ESRI ASCII-raster data set
		Raster(const std::string& FileName) : m_statistic_actual(false),m_statistic()
		{
			std::ifstream ASCFile;
			try
			{
				ASCFile.open(FileName.c_str());
				if (!ASCFile) throw std::runtime_error("Raster file: " + FileName + " not found");
			}
			catch (...)
			{
				ASCFile.close();
				throw std::runtime_error("Could not read Raster file: " + FileName);
			}
			m_Header=header<rasterType>(ASCFile);
			LoadFromASC(ASCFile);
			ASCFile.close();
		}

		/// Loads an ESRI ASCII-raster data set
		Raster(std::istream& ASCFile) : m_statistic_actual(false),m_statistic()
		{
			m_Header=header<rasterType>(ASCFile);
			LoadFromASC(ASCFile);
		}
		/// Writes the raster to a stream in ESRI-ASC format
		void WriteToASCFile(std::ostream& ASCFile)
		{
			m_Header.WriteToStream(ASCFile);
			data::iterator reader=m_data.begin();
			for (int row=0;row<this->m_Header.nrows;row++)
			{
				for (int col=0;col<this->m_Header.ncols;col++)
					ASCFile << *reader++ << " ";
				ASCFile << std::endl;
			}
		}
		/// Writes the raster to a filename
		void WriteToASCFile(std::string filename)
		{
			std::ofstream file;
			try
			{
				file.open(filename.c_str());
				if (!file) throw std::ofstream::failure("Raster file: " + filename + " could not be created");
			}
			catch (...)
			{
				file.close();
				throw std::ofstream::failure("Raster file: " + filename + " could not be created");
			}
			WriteToASCFile(file);
			file.close();
		}
		/// Writes the data to the file with the given file name and the header to a filename with the extension .hdr
		/// @note If the filename has the extension .flt and the raster is a float raster the saved file can be read by ArcGIS
		void WriteToBinary(std::string filename) const
		{
			size_t dotpos=filename.find_last_of('.');
			std::string hdrfilename=filename.substr(0,dotpos) + ".hdr";
			std::ofstream hdrfile;
			try
			{
				hdrfile.open(hdrfilename.c_str());
				if (!hdrfile) throw std::ofstream::failure("Raster file: " + hdrfilename + " could not be created");
			}
			catch (...)
			{
				hdrfile.close();
				throw std::ofstream::failure("Raster file: " + hdrfilename + " could not be created");
			}
			m_Header.WriteToStream(hdrfile);
			hdrfile << "BYTEORDER MSBFIRST" << std::endl;
			hdrfile.close();
			std::ofstream binfile;
			binfile.open(filename.c_str(),std::ios_base::binary || std::ios_base::out);
			for(data::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
			{
				binfile.write((const char*)&(*it),sizeof(rasterType));
			}
			binfile.close();
		}

#ifndef SWIG
		Raster& operator=(const Raster<rasterType>& R)
		{
			int oldcol=ColumnCount(),oldrow=RowCount();
			if (oldcol*oldrow!=R.ColumnCount()*R.RowCount())
			{
				m_data=data(R.ColumnCount()*R.RowCount());
			}
			m_Header = header<rasterType>(R.ColumnCount(),R.RowCount(),R.Xllcorner(),R.Yllcorner(),R.XCellsize(),R.YCellsize(),R.NoData());
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
				for (int c = 0; c < m_Header.ncols ; c++)
					m_data[r*m_Header.ncols+c]=R.IdentifyColRow(c,r);

			return *this;
		}
#endif
		//@}

		///@name Conversion functions
		//@{
		/// Converts the raster to a raster of int
		Raster<int> ToInt() const
		{
			Raster<int> result(ColumnCount(),RowCount(),Xllcorner(),Yllcorner(),XCellsize(),YCellsize(),int(NoData()),0);
#pragma omp parallel for
			for (int r = 0; r < (int)RowCount() ; ++r)
				for (int c = 0; c < ColumnCount() ; ++c)
					result.SetData(c,r,int(GetData(c,r)+0.5));
			return result;
		}		

		/// Converts the raster to a raster of float (32bit)
		Raster<float> ToFloat()
		{
			Raster<float> result(ColumnCount(),RowCount(),Xllcorner(),Yllcorner(),XCellsize(),YCellsize(),float(NoData()),0);
#pragma omp parallel for
			for (int r = 0; r < (int)RowCount() ; ++r)
				for (int c = 0; c < ColumnCount() ; ++c)
					result.SetData(c,r,float(GetData(c,r)));
			return result;
		}
		/// Converts the raster to a raster of float (64bit)
		Raster<double> ToDouble()
		{
			Raster<double> result(ColumnCount(),RowCount(),Xllcorner(),Yllcorner(),XCellsize(),YCellsize(),double(NoData()),0);
#pragma omp parallel for
			for (int r = 0; r < (int)RowCount() ; ++r)
				for (int c = 0; c < ColumnCount() ; ++c)
					result.SetData(c,r,double(GetData(c,r)));
			return result;
		}
    /// Returns the memory adress of the data
    long long adress() const
    {
        return (long long)(&m_data[0]);
    }
		//@}

		/// @name Focal functions
		//@{
	private:
#ifndef SWIG
		struct Window
		{
		private:
		public:
			int center_row,center_col,windowsize;
			int begin() const {return 0;}
			int end() const {return windowsize*windowsize;}
			int col(int index) const
			{
				return center_col+index % windowsize - (windowsize-1)/2;
			}
			int row(int index) const
			{
				return center_row+index/windowsize - (windowsize-1)/2;
			}
			Window(int _col,int _row,int _windowsize) : center_row(_row),center_col(_col),windowsize(_windowsize) {}
		};
		rasterType querywindow(const Window & window,int index)
		{
			return GetData(window.col(index),window.row(index));
		}
		// All window_* functions analyse a data window
		rasterType window_mean(const Window & window)
		{
			rasterType mean=rasterType(0);
			int windowcount=0;
			for(int it = window.begin(); it != window.end(); ++it)
			{
				rasterType d=querywindow(window,it);
				if (d!=NoData())
				{
					mean+=d;
					++windowcount;
				}
			}
			if (windowcount)
				return mean/windowcount;
			else
				return NoData();
		}
		rasterType window_max(const Window & window)
		{
			rasterType max=NoData();
			for(int it = window.begin(); it != window.end(); ++it)
			{
				rasterType d=querywindow(window,it);
				if (d!=NoData())
				{
					max = max==NoData() || d>max ? d : max;
				}
			}
			return max;
		}
		rasterType window_min(const Window & window)
		{
			rasterType min=NoData();
			for(int it = window.begin(); it != window.end(); ++it)
			{
				rasterType d=querywindow(window,it);
				if (d!=NoData())
				{
					min = min==NoData() || d<min ? d : min;
				}
			}
			return min;
		}
		rasterType window_stdev(const Window & window)
		{
			rasterType sq_sum=0,sum=0;
			int windowcount=0;
			for(int it = window.begin(); it != window.end(); ++it)
			{
				rasterType d=querywindow(window,it);
				if (d!=NoData())
				{
					sum+=d;
					sq_sum+=d * d;
					++windowcount;
				}
			}
			if (windowcount)
				return rasterType(sqrt(double(sq_sum/windowcount - sum/windowcount * sum/windowcount)));
			else
				return NoData();

		}
		rasterType window_majority(const Window & window)
		{
			int count=1,max_count=1;
			rasterType majority=querywindow(window,(window.end()-1)/2);
			for(int it = window.begin(); it != window.end(); ++it)
			{
				count=1;
				rasterType d=querywindow(window,it);
				if (d!=NoData())
				{
					for(int it_cmp = window.begin(); it_cmp != window.end(); ++it_cmp)
						if (d==querywindow(window,it_cmp))
							++count;
					if (count>max_count)
					{
						max_count=count;
						majority=d;
					}
				}
			}
			return majority;
		}
		rasterType window_mean_difference(const Window & window)
		{
			return abs(window_mean(window)-GetData(window.center_col,window.center_row));
		}

		void focal(Raster<rasterType> & result,int window_function,int WindowSize)
		{
#pragma omp parallel for
			for (int r = 0; r < m_Header.nrows ; r++)
			{
				Window window(0,0,WindowSize);
				for (int c = 0; c < m_Header.ncols ; c++)
				{
					if (this->HasData(c,r))
					{
						window.center_col=c;
						window.center_row=r;

						rasterType val=
							window_function==1 ? window_mean(window) :
							window_function==2 ? window_min(window) :
							window_function==3 ? window_max(window) :
							window_function==4 ? window_majority(window) :
							window_function==5 ? window_stdev(window) :
							window_function==6 ? window_mean_difference(window) : NoData() ;
						result.SetData(c,r,val);
					}
					else
						result.SetData(c,r,this->NoData());
				}
			}
		}
#endif
	public:
		/// Creates a raster, which contains for each cell the minimum of the surrounding n x n window
		Raster<rasterType> focal_min(int n=3)
		{
			Raster<rasterType> result(*this,rasterType(0));
			focal(result,2,n);
			return result;
		}
		/// Creates a raster, which contains for each cell the maximum of the surrounding n x n window
		Raster<rasterType> focal_max(int n=3)
		{
			Raster<rasterType> result(*this,rasterType(0));
			focal(result,3,n);
			return result;
		}
		/// Creates a raster, which contains for each cell the mean of the surrounding n x n window
		Raster<rasterType> focal_mean(int n=3)
		{
			Raster<rasterType> result(*this,rasterType(0));
			focal(result,1,n);
			return result;
		}
		/// Creates a raster, which contains for each cell the standard deviation of the surrounding n x n window
		Raster<rasterType> focal_stdev(int n=3)
		{
			Raster<rasterType> result(*this,rasterType(0));
			focal(result,5,n);
			return result;
		}
		/// Creates a raster, which contains for each cell the majority value of the surrounding n x n window (usually only used for integer raster)
		Raster<rasterType> focal_majority(int n=3)
		{
			Raster<rasterType> result(*this,rasterType(0));
			focal(result,4,n);
			return result;
		}
		/// Creates a raster, which contains for each cell the difference between the actual value and the mean of the surrounding n x n window (usually only used for integer raster)
		/// This function can be used to identify very important points (VIP)	for triangulation
		Raster<rasterType> focal_mean_difference(int n=3)
		{
			Raster<rasterType> result(*this,rasterType(0));
			focal(result,6,n);
			return result;
		}
		//@}
		Raster<rasterType> clone() const
		{
			return Raster<rasterType>(*this);
		}
		/// Reads the data at many places
		std::vector<rasterType> GetData(std::vector<double> x,std::vector<double> y)
		{
			if (x.size()!=y.size()) throw std::runtime_error("Multiple raster read: x and y differ in size");
			std::vector<rasterType> res;				
#pragma omp parallel for
			for (int i = 0; i < (int)x.size() ; ++i)
			{
				res.push_back(this->GetData(x[i],y[i]));		
			}
			return res;
		}

		std::string ToBuffer() const
		{
			
			std::stringstream buffer;
			std::streamsize dsize=sizeof(m_data[0]);
			for(data::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
			{
			    buffer.write((char*)(&(*it)),dsize);
			}
			return buffer.str();
		}
	};
}




#endif // RasterTemplate_h__

