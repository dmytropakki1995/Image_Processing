import { ImageAction } from "../types/action"
import { ImageLoaded, StartImageProcessing, GotProcessingResult, GotProcessingError, SetProcessingType } from "../constants/action"
import { combineReducers } from 'redux';


interface State {
    image: any;
    isProcessing: boolean;
    processingType: number;
    error: string;
    porcessingResult: any;
}

const initialState: State = {
    image: null,
    isProcessing: false,
    processingType: 0,
    error: '',
    porcessingResult: null
}

const image = (state = initialState.image, action: ImageAction): any => {
    switch (action.type) {
        case ImageLoaded:
            return action.payload;
        case GotProcessingResult:
            return initialState.image;
        case StartImageProcessing:
        case GotProcessingError:
        default:
            return state;
    }
}

const isProcessing = (state = initialState.isProcessing, action: ImageAction): boolean => {
    switch (action.type) {
        case StartImageProcessing:
            return true;
        case ImageLoaded:
        case GotProcessingResult:
        case GotProcessingError:
            return false;
        default:
            return state;
    }
}

const processingType = (state = initialState.processingType, action: ImageAction): number => {
    switch (action.type) {
        case SetProcessingType:
            return action.payload;
        default:
            return state;
    }
}

const error = (state = initialState.error, action: ImageAction): string => {
    switch (action.type) {
        case GotProcessingError:
            return action.payload;
        default:
            return initialState.error;
    }
}

const porcessingResult = (state = initialState.error, action: ImageAction): any => {
    switch (action.type) {
        case GotProcessingError:
        case ImageLoaded:
        case StartImageProcessing:
            return null;
        case GotProcessingResult:
            return action.payload;
        default:
            return state;
    }
}

export const getProcessingError = (state: State) => state.error;
export const getImage = (state: State) => state.image;
export const getProcessingResult = (state: State) => state.porcessingResult;
export const getIsProcessing = (state: State) => state.isProcessing;

export default combineReducers({ image, isProcessing, processingType, error, porcessingResult });