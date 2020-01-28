import { ImageLoaded, StartImageProcessing, GotProcessingResult, GotProcessingError} from "../constants/action";

export interface ImageLoadedAction {
    type: typeof ImageLoaded;
    payload: any;
}

export interface StartImageProcessingAction {
    type: typeof StartImageProcessing;
    payload: number;
}

export interface GotProcessingResultAction {
    type: typeof GotProcessingResult
    payload: any;
}

export interface GotProcessingErrorAction {
    type: typeof GotProcessingError;
    payload: string;
}

export type ImageAction = ImageLoadedAction | StartImageProcessingAction | GotProcessingResultAction | GotProcessingErrorAction;
