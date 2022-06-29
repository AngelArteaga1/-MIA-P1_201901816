export class Tree {
    constructor(
        public name: string,
        public type: string,
        public size: string,
        public content: string,
        public sons: Tree[]
    ) {}
}
