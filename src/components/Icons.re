module Close = {
	[@react.component]
let make = (~className="") =>
  <svg
    className
    width="35"
    height="35"
    viewBox="0 0 35 35"
    fill="none"
    xmlns="http://www.w3.org/2000/svg">
    <path
      d="M24.043 17.25L33.8086 27.0156C34.3945 27.6016 34.6875 28.3828 34.6875 29.1641C34.6875 30.043 34.3945 30.8242 33.8086 31.4102L31.6602 33.5586C30.9766 34.1445 30.1953 34.4375 29.4141 34.4375C28.5352 34.4375 27.8516 34.1445 27.2656 33.5586L17.5 23.793L7.73438 33.5586C7.14844 34.1445 6.36719 34.4375 5.58594 34.4375C4.70703 34.4375 3.92578 34.1445 3.33984 33.5586L1.19141 31.4102C0.605469 30.8242 0.3125 30.043 0.3125 29.1641C0.3125 28.3828 0.605469 27.6016 1.19141 27.0156L10.957 17.25L1.19141 7.48438C0.605469 6.89844 0.3125 6.21484 0.3125 5.33594C0.3125 4.55469 0.605469 3.77344 1.19141 3.08984L3.33984 0.941406C3.92578 0.355469 4.70703 0.0625 5.58594 0.0625C6.36719 0.0625 7.14844 0.355469 7.73438 0.941406L17.5 10.707L27.2656 0.941406C27.8516 0.355469 28.5352 0.0625 29.4141 0.0625C30.1953 0.0625 30.9766 0.355469 31.6602 0.941406L33.8086 3.08984C34.3945 3.77344 34.6875 4.55469 34.6875 5.33594C34.6875 6.21484 34.3945 6.89844 33.8086 7.48438L24.043 17.25Z"
      fill="#989898"
    />
  </svg>;
}