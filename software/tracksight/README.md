# Tracksight

## Setup

### Backend
This sets up the conda environment (you will also need to install conda) for running the backend locally.
#### Initial Environment Setup:
1. Install conda
2. Run `conda env create --file environment.yml`
3. Activate environment using `conda activate tracksight-backend`

#### Updating Environment Setup:
1. Activate environment using `conda activate tracksight-backend`
2. Update environment using `conda env update --file environment.yml --prune`

## MVP
Build a service to create a dashboard with signals

- Livestream a signal in realtime
- Read a signal from an sd card 
- Build a dashboard

### Backend
- data storage (mocked)
- data inflow( mocked)
- From real sd card data

## Future
- Better data visualizations
- testing luhmao :)
- Addtional input formats
- Interacting with cloud data
- Cloud data blah blah

## Schema
Client request:
{
ids: [“1234”, “5678”]
}

Response:
{
	“1234”: {
		“Name”: str,
		“timestamp”:
		“data”: df
	},
5678
}

“1234” + “5678”

{
Op: “ad”
Args: []

}
